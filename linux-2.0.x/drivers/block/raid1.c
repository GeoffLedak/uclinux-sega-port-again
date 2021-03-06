/************************************************************************
 * raid1.c : Multiple Devices driver for Linux
 *           Copyright (C) 1996 Ingo Molnar, Miguel de Icaza, Gadi Oxman
 *
 * RAID-1 management functions.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example /usr/src/linux/COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/locks.h>
#include <linux/malloc.h>
#include <linux/md.h>
#include <linux/raid1.h>
#include <asm/bitops.h>
#include <asm/atomic.h>

#define MAJOR_NR MD_MAJOR
#define MD_DRIVER
#define MD_PERSONALITY

/*
 * The following can be used to debug the driver
 */
/*#define RAID1_DEBUG*/
#ifdef RAID1_DEBUG
#define PRINTK(x)   do { printk x; } while (0);
#else
#define PRINTK(x)   do { ; } while (0);
#endif


static struct md_personality raid1_personality;
static struct md_thread *raid1_thread = NULL;
struct buffer_head *raid1_retry_list = NULL;

static int __raid1_map (struct md_dev *mddev, kdev_t *rdev,
		        unsigned long *rsector, unsigned long size)
{
	struct raid1_data *raid_conf = (struct raid1_data *) mddev->private;
	int i, n = raid_conf->raid_disks;

	/*
	 * Later we do read balancing on the read side 
	 * now we use the first available disk.
	 */

	PRINTK(("raid1_map().\n"));

	for (i=0; i<n; i++) {
		if (raid_conf->mirrors[i].operational) {
			*rdev = raid_conf->mirrors[i].dev;
			return (0);
		}
	}

	printk (KERN_ERR "raid1_map(): huh, no more operational devices?\n");
	return (-1);
}

static int raid1_map (struct md_dev *mddev, kdev_t *rdev,
		      unsigned long *rsector, unsigned long size)
{
	return 0;
}

void raid1_reschedule_retry (struct buffer_head *bh)
{
	struct raid1_bh * r1_bh = (struct raid1_bh *)(bh->private_bh);

	PRINTK(("raid1_reschedule_retry().\n"));

	r1_bh->next_retry = raid1_retry_list;
	raid1_retry_list = bh;
	md_wakeup_thread(raid1_thread);
}

/*
 * raid1_end_buffer_io() is called when we have finished servicing a mirrored
 * operation and are ready to return a success/failture code to the buffer
 * cache layer.
 */
static inline void raid1_end_buffer_io (struct buffer_head *bh, int uptodate)
{
	/*
	 * kfree() can sleep and we try to keep this bh operation atomic.
	 */
	struct raid1_bh * tmp = (struct raid1_bh *) bh->private_bh;

	clear_bit (BH_MD, &bh->b_state);
	bh->private_bh = NULL;
	bh->personality = NULL;
	mark_buffer_uptodate(bh, uptodate);
	unlock_buffer(bh);
	kfree(tmp);
}

void raid1_end_request (struct buffer_head *bh, int uptodate)
{
	struct raid1_bh * r1_bh = (struct raid1_bh *)(bh->private_bh);
	unsigned long flags;

	save_flags(flags);
	cli();
	PRINTK(("raid1_end_request().\n"));

	/*
	 * this branch is our 'one mirror IO has finished' event handler:
	 */
	if (!uptodate)
		md_error (bh->b_dev, bh->b_rdev);
	else {
		/*
		 * Set BH_Uptodate in our master buffer_head, so that
		 * we will return a good error code for to the higher
		 * levels even if IO on some other mirrored buffer fails.
		 *
		 * The 'master' represents the complex operation to 
		 * user-side. So if something waits for IO, then it will
		 * wait for the 'master' buffer_head.
		 */
		set_bit (BH_Uptodate, &r1_bh->state);
	}

	/*
	 * We split up the read and write side, imho they are 
	 * conceptually different.
	 */

	if ( (r1_bh->cmd == READ) || (r1_bh->cmd == READA) ) {

		PRINTK(("raid1_end_request(), read branch.\n"));

		/*
		 * we have only one buffer_head on the read side
		 */
		if (uptodate) {
			PRINTK(("raid1_end_request(), read branch, uptodate.\n"));
			raid1_end_buffer_io (bh, uptodate);
			restore_flags(flags);
			return;
		}
		/*
		 * oops, read error:
		 */
		printk(KERN_ERR "raid1: %s: rescheduling block %lu\n", 
				 kdevname(bh->b_dev), bh->b_blocknr);
		raid1_reschedule_retry (bh);
		restore_flags(flags);
		return;
	}

	/*
	 * WRITE or WRITEA.
	 */
	PRINTK(("raid1_end_request(), write branch.\n"));

	/*
	 * lets see if all mirrored write operations have finished 
	 * already [we have irqs off, so we can decrease]:
	 */

	if (!--r1_bh->remaining) {
		struct md_dev *mddev = r1_bh->mddev;
		struct raid1_data *raid_conf = (struct raid1_data *) mddev->private;
		int i, n = raid_conf->raid_disks;

		PRINTK(("raid1_end_request(), remaining == 0.\n"));

		/*
		 * kfree() can sleep? really? if yes then we are
		 * doomed here ...
		 */
		for ( i=0; i<n; i++) {
			if (r1_bh->mirror_bh[i]) kfree(r1_bh->mirror_bh[i]);
		}

		/*
		 * the 'master' bh is the one that is used in page IO,
		 * perhaps someone is waiting on it. Lets erase all
		 * signs of mirroring, and lets finish the bh operation:
		 *
		 * In particular, the "uptodate" value which we return
		 * to the higher level represents the entire mirror set.
		 *
		 * yes, and this is why i want to use the 'master' bh as
		 * a 'representative'. Thats why i think it's not clean to
		 * use the master bh for real IO. We mix concepts, which
		 * isnt too good.
		 *
		 * a buffer_head is basically a user-side file buffer.
		 * Normally it has direct relationship with the physical
		 * device, but as in this case, we have an abstract mapping
		 * between the file buffer and the physical layout. So i've
		 * reverted all changes that do this mixing.
		 *
		 * we 'waste' about 76 bytes for the one more buffer_head,
		 * but note that we will do the mirror bh allocation at once
		 * in the future, so this isnt really a valid point, i think.
		 *
		 * Also i dont like the current way of mixing the user-side buffer
		 * concept with the 'real' physical layout like raid0.c does
		 * now: it increases the size of buffer_head even for nonstriped
		 * devices, etc.
		 *
		 * IMHO, in the future, we should have a lightweight buffer_head
		 * structure, which holds almost no physical device information.
		 
		 * Abstract relationship between buffers:
		 * ===================================== 
		 * 
		 *           [user] 
		 *              |
		 *              |
		 *    ['master' buffer_head] + [private_buffer_head]
		 *                                      |
		 *                                      |
		 *                                      |
		 *                        [additional 'sub'-buffer_heads]
		 *                           |          |           | 
		 *                         [dev1]     [dev2]      [dev3]
		 * 
 
		 * In this scheme it's not clean to use the 'master' as one of
		 * the 'sub' buffer_heads. If you think about it, currently we can 
		 * do this only because raid0 introduced it's own private_buffer_head
		 * structure in buffer_head: rdev,rsector. And raid0 has a 1:1
		 * relationship to the physical device. But this is really just a
		 * special case. Once we have our megafast bh pools running, we could
		 * clean up raid0.c too :))
		 *
		 * Not that it isnt clean, it is lethal if in the future we insert our 
		 * sub buffer_heads into the global block cache. The master request
		 * should be an IO operation label for the complex operation, nothing
		 * more.
		 *
		 * So we have almost no performance arguments, and alot of cleanness
		 * arguments.
		 *
		 * Comments? Gonna change it back to your way again if you can convince
		 * me :)) --mingo
		 *
		 */
		raid1_end_buffer_io ( r1_bh->master_bh, 
				test_bit (BH_Uptodate, &r1_bh->state));
	}
	else PRINTK(("raid1_end_request(), remaining == %u.\n", r1_bh->remaining));
	restore_flags(flags);
}

/* This routine checks if the undelying device is an md device and in that
 * case it maps the blocks before putting the request on the queue
 */
static inline void
map_and_make_request (int rw, struct buffer_head *bh)
{
	if (MAJOR (bh->b_rdev) == MD_MAJOR){
		md_map (MINOR (bh->b_rdev), &bh->b_rdev, &bh->b_rsector, bh->b_size >> 9);
	}
	make_request (MAJOR (bh->b_rdev), rw, bh);
}
	
static int
raid1_make_request (struct md_dev *mddev, int rw, struct buffer_head * bh)
{

	struct raid1_data *raid_conf = (struct raid1_data *) mddev->private;
	struct buffer_head *mirror_bh[MD_SB_DISKS];
	struct raid1_bh * r1_bh;
	int n = raid_conf->raid_disks, i, sum_bhs = 0, switch_disks = 0, sectors;
	struct mirror_info *mirror;

	PRINTK(("raid1_make_request().\n"));

/*
 * We put allocations at the beginning, to avoid sleeping while doing
 * atomic operations of buffer heads. This might or might not make much
 * difference, but lets rather be careful.
 *
 * but this has two side effects (probably non harmless):
 *
 *	1.	The buffer will not be locked while we sleep.
 *	2.	The rest of the kernel will see BH_Req without
 *		BH_Lock.
 */
	while (!( /* FIXME: now we are rather fault tolerant than nice */
	r1_bh = kmalloc (sizeof (struct raid1_bh), GFP_KERNEL)
	) )
		printk ("raid1_make_request(#1): out of memory\n");
	memset (r1_bh, 0, sizeof (struct raid1_bh));
/*
 * make_request() can abort the operation when READA or WRITEA are being
 * used and no empty request is available.
 *
 * Currently, just replace the command with READ/WRITE.
 */
	if (rw == READA) rw = READ;
	if (rw == WRITEA) rw = WRITE;

	if (rw == WRITE || rw == WRITEA)
		mark_buffer_clean(bh);		/* Too early ? */

/*
 * i think the read and write branch should be separated completely, since we want
 * to do read balancing on the read side for example. Comments? :) --mingo
 */

	r1_bh->master_bh=bh;
	r1_bh->mddev=mddev;
	r1_bh->cmd = rw;

	set_bit (BH_MD,	&bh->b_state);
	bh->personality  = &raid1_personality;
	bh->private_bh   = (void*)(r1_bh);

	if (rw==READ || rw==READA) {
		int last_used = raid_conf->last_used;
		PRINTK(("raid1_make_request(), read branch.\n"));
		mirror = raid_conf->mirrors + last_used;
		bh->b_rdev = mirror->dev;
		sectors = bh->b_size >> 9;
		if (bh->b_blocknr * sectors == raid_conf->next_sect) {
			raid_conf->sect_count += sectors;
			if (raid_conf->sect_count >= mirror->sect_limit)
				switch_disks = 1;
		} else
			switch_disks = 1;
		raid_conf->next_sect = (bh->b_blocknr + 1) * sectors;
		if (switch_disks) {
			PRINTK(("read-balancing: switching %d -> %d (%d sectors)\n", last_used, mirror->next, raid_conf->sect_count));
			raid_conf->sect_count = 0;
			raid_conf->last_used = mirror->next;
		}
		PRINTK (("raid1 read queue: %d %d\n", MAJOR (bh->b_rdev), MINOR (bh->b_rdev)));

		clear_bit (BH_Lock, &bh->b_state);
		map_and_make_request (rw, bh);
		return 0;
	}

	/*
	 * WRITE or WRITEA.
	 */
/*
 * btw, we have no more master disk. 'slave' is gone too :) [i hate that word :))]
 *
 * We are now using the master bh for a real IO. It seems important that:
 *
 * 1.	lock_buffer() will be called when we start to handle the request,
 *	before we do anything (done by ll_rw_blk.c).
 *
 * 2.	It seems that Linus took great care to set mark_buffer_clean()
 *	atomically with cli() in effect just when the buffer was placed
 *	into the queue. To be compatible with this behavior, it would be
 *	best to lock the buffer *first*, but mark it clean *last*, and to
 *	do this by passing through the exact logic in ll_rw_blk.c.
 *
 * Note: i've reverted this #3 thing, see the big comment in this file.
 *
 * 3.	We are now called from within make_request(), so the real bh
 *	will be automatically handled last when we return, so we only need
 *	to add the rest of the buffers (but remember to include the
 *	master bh in the remaining count).
 */
	PRINTK(("raid1_make_request(n=%d), write branch.\n",n));

	for (i = 0; i < n; i++) {

		if (!raid_conf->mirrors [i].operational) {
			/*
			 * the r1_bh->mirror_bh[i] pointer remains NULL
			 */
			mirror_bh[i] = NULL;
			continue;
		}

	/*
	 * We should use a private pool (size depending on NR_REQUEST),
	 * to avoid writes filling up the memory with bhs
	 *
	 * Such pools are much faster than kmalloc anyways (so we waste almost 
	 * nothing by not using the master bh when writing and win alot of cleanness)
	 *
	 * but for now we are cool enough. --mingo
	 *
	 * It's safe to sleep here, buffer heads cannot be used in a shared
	 * manner in the write branch. Look how we lock the buffer at the beginning
	 * of this function to grok the difference ;)
	 */
		while (!( /* FIXME: now we are rather fault tolerant than nice */
		mirror_bh[i] = kmalloc (sizeof (struct buffer_head), GFP_KERNEL)
		) )
			printk ("raid1_make_request(#2): out of memory\n");
		memset (mirror_bh[i], 0, sizeof (struct buffer_head));

	/*
	 * prepare mirrored bh (fields ordered for max mem throughput):
	 */
		mirror_bh [i]->b_blocknr    = bh->b_blocknr;
		mirror_bh [i]->b_dev        = bh->b_dev;
		mirror_bh [i]->b_rdev 	    = raid_conf->mirrors [i].dev;
		mirror_bh [i]->b_rsector    = bh->b_rsector;
		mirror_bh [i]->b_state      =   (1<<BH_MD)      | (1<<BH_Req) | 
						(1<<BH_Touched) | (1<<BH_Dirty);
		mirror_bh [i]->b_count      = 1;
		mirror_bh [i]->b_size       = bh->b_size;
		mirror_bh [i]->b_data       = bh->b_data;
		mirror_bh [i]->b_list       = BUF_LOCKED;
		mirror_bh [i]->personality  = &raid1_personality;
		mirror_bh [i]->private_bh   = (void*)(r1_bh);

		r1_bh->mirror_bh[i] = mirror_bh[i];
		sum_bhs++;
	}

	r1_bh->remaining = sum_bhs;

	PRINTK(("raid1_make_request(), write branch, sum_bhs=%d.\n",sum_bhs));

	/*
	 * We have to be a bit careful about the semaphore above, thats why we
	 * start the requests separately. Since kmalloc() could fail, sleep and
	 * make_request() can sleep too, this is the safer solution. Imagine,
	 * end_request decreasing the semaphore before we could have set it up ...
	 * We could play tricks with the semaphore (presetting it and correcting
	 * at the end if sum_bhs is not 'n' but we have to do end_request by hand
	 * if all requests finish until we had a chance to set up the semaphore
	 * correctly ... lots of races).
	 */
	for (i = 0; i < n; i++)
		if (mirror_bh [i] != NULL)
			map_and_make_request (rw, mirror_bh [i]);

	return (0);
}
			   
static int raid1_status (char *page, int minor, struct md_dev *mddev)
{
	struct raid1_data *raid_conf = (struct raid1_data *) mddev->private;
	int sz = 0, i;
	
	sz += sprintf (page+sz, " [%d/%d] [", raid_conf->raid_disks, raid_conf->working_disks);
	for (i = 0; i < raid_conf->raid_disks; i++)
		sz += sprintf (page+sz, "%s", raid_conf->mirrors [i].operational ? "U" : "_");
	sz += sprintf (page+sz, "]");
	return sz;
}

static void raid1_fix_links (struct raid1_data *raid_conf, int failed_index)
{
	int disks = raid_conf->raid_disks;
	int j;

	for (j = 0; j < disks; j++)
		if (raid_conf->mirrors [j].next == failed_index)
			raid_conf->mirrors [j].next = raid_conf->mirrors [failed_index].next;
}

static int raid1_error (struct md_dev *mddev, kdev_t dev)
{
	struct raid1_data *raid_conf = (struct raid1_data *) mddev->private;
	struct mirror_info *mirror;
	md_superblock_t *sb = mddev->sb;
	int disks = raid_conf->raid_disks;
	int i;

	PRINTK(("raid1_error called\n"));

	if (raid_conf->working_disks == 1) {
		/*
		 * Uh oh, we can do nothing if this is our last disk, but
		 * first check if this is a queued request for a device
		 * which has just failed.
		 */
		for (i = 0, mirror = raid_conf->mirrors; i < disks; i++, mirror++)
			if (mirror->dev == dev && !mirror->operational)
				return 0;
		printk (KERN_ALERT "RAID1: only one disk left and IO error.\n");
		return 0;
	}

	/* Mark disk as unusable */
	for (i = 0, mirror = raid_conf->mirrors; i < disks; i++, mirror++) {
		if (mirror->dev == dev && mirror->operational){
			mirror->operational = 0;
			raid1_fix_links (raid_conf, i);
			sb->disks[mirror->number].state |= (1 << MD_FAULTY_DEVICE);
			sb->disks[mirror->number].state &= ~(1 << MD_SYNC_DEVICE);
			sb->disks[mirror->number].state &= ~(1 << MD_ACTIVE_DEVICE);
			sb->active_disks--;
			sb->working_disks--;
			sb->failed_disks++;
			mddev->sb_dirty = 1;
			md_wakeup_thread(raid1_thread);
			raid_conf->working_disks--;
			printk (KERN_ALERT
				"RAID1: Disk failure on %s, disabling device."
				"Operation continuing on %d devices\n",
				kdevname (dev), raid_conf->working_disks);
		}
	}

	return 0;
}

/*
 * This is a kernel thread which:
 *
 *	1.	Retries failed read operations on working mirrors.
 *	2.	Updates the raid superblock when problems are encountered.
 */
void raid1d (void *data)
{
	struct buffer_head *bh;
	kdev_t dev;
	unsigned long flags;
	struct raid1_bh * r1_bh;
	struct md_dev *mddev;

	PRINTK(("raid1d() active\n"));
	save_flags(flags);
	cli();
	while (raid1_retry_list) {
		bh = raid1_retry_list;
		r1_bh = (struct raid1_bh *)(bh->private_bh);
		raid1_retry_list = r1_bh->next_retry;
		restore_flags(flags);

		mddev = md_dev + MINOR(bh->b_dev);
		if (mddev->sb_dirty) {
			mddev->sb_dirty = 0;
			md_update_sb(MINOR(bh->b_dev));
		}
		dev = bh->b_rdev;
		__raid1_map (md_dev + MINOR(bh->b_dev), &bh->b_rdev, &bh->b_rsector, bh->b_size >> 9);
		if (bh->b_rdev == dev) {
			printk (KERN_ALERT 
					"raid1: %s: unrecoverable I/O read error for block %lu\n",
						kdevname(bh->b_dev), bh->b_blocknr);
			raid1_end_buffer_io (bh, 0);
		} else {
			printk (KERN_ERR "raid1: %s: redirecting sector %lu to another mirror\n", 
					  kdevname(bh->b_dev), bh->b_blocknr);
			clear_bit (BH_Lock, &bh->b_state);
			map_and_make_request (r1_bh->cmd, bh);
		}
		cli();
	}
	restore_flags(flags);
	
}

/*
 * This will catch the scenario in which one of the mirrors was
 * mounted as a normal device rather than as a part of a raid set.
 */
static int check_consistenty (struct md_dev *mddev)
{
	struct raid1_data *raid_conf = mddev->private;
	kdev_t dev;
	struct buffer_head *bh = NULL;
	int i, rc = 0;
	char *buffer = NULL;

	for (i = 0; i < raid_conf->raid_disks; i++) {
		if (!raid_conf->mirrors[i].operational)
			continue;
		dev = raid_conf->mirrors[i].dev;
		set_blocksize(dev, 4096);
		if ((bh = bread(dev, 0, 4096)) == NULL)
			break;
		if (!buffer) {
			buffer = (char *) __get_free_page(GFP_KERNEL);
			if (!buffer)
				break;
			memcpy(buffer, bh->b_data, 4096);
		} else if (memcmp(buffer, bh->b_data, 4096)) {
			rc = 1;
			break;
		}
		bforget(bh);
		fsync_dev(dev);
		invalidate_buffers(dev);
		bh = NULL;
	}
	if (buffer)
		free_page((unsigned long) buffer);
	if (bh) {
		dev = bh->b_dev;
		bforget(bh);
		fsync_dev(dev);
		invalidate_buffers(dev);
	}
	return rc;
}

static int raid1_run (int minor, struct md_dev *mddev)
{
	struct raid1_data *raid_conf;
	int i, j, raid_disk;
	md_superblock_t *sb = mddev->sb;
	md_descriptor_t *descriptor;
	struct real_dev *realdev;

	MOD_INC_USE_COUNT;

	if (sb->level != 1) {
		printk("raid1: %s: raid level not set to mirroring (%d)\n", kdevname(MKDEV(MD_MAJOR, minor)), sb->level);
		MOD_DEC_USE_COUNT;
		return -EIO;
	}
	/****
	 * copy the now verified devices into our private RAID1 bookkeeping area:
	 *
	 * [whatever we allocate in raid1_run(), should be freed in raid1_stop()]
	 */

	while (!( /* FIXME: now we are rather fault tolerant than nice */
	mddev->private = kmalloc (sizeof (struct raid1_data), GFP_KERNEL)
	) )
		printk ("raid1_run(): out of memory\n");
	raid_conf = mddev->private;
	memset(raid_conf, 0, sizeof(*raid_conf));

	PRINTK(("raid1_run(%d) called.\n", minor));

  	for (i = 0; i < mddev->nb_dev; i++) {
  		realdev = &mddev->devices[i];
		if (!realdev->sb) {
			printk(KERN_ERR "raid1: disabled mirror %s (couldn't access raid superblock)\n", kdevname(realdev->dev));
			continue;
		}

		/*
		 * This is important -- we are using the descriptor on
		 * the disk only to get a pointer to the descriptor on
		 * the main superblock, which might be more recent.
		 */
		descriptor = &sb->disks[realdev->sb->descriptor.number];
		if (descriptor->state & (1 << MD_FAULTY_DEVICE)) {
			printk(KERN_ERR "raid1: disabled mirror %s (errors detected)\n", kdevname(realdev->dev));
			continue;
		}
		if (descriptor->state & (1 << MD_ACTIVE_DEVICE)) {
			if (!(descriptor->state & (1 << MD_SYNC_DEVICE))) {
				printk(KERN_ERR "raid1: disabled mirror %s (not in sync)\n", kdevname(realdev->dev));
				continue;
			}
			raid_disk = descriptor->raid_disk;
			if (descriptor->number > sb->nr_disks || raid_disk > sb->raid_disks) {
				printk(KERN_ERR "raid1: disabled mirror %s (inconsistent descriptor)\n", kdevname(realdev->dev));
				continue;
			}
			if (raid_conf->mirrors[raid_disk].operational) {
				printk(KERN_ERR "raid1: disabled mirror %s (mirror %d already operational)\n", kdevname(realdev->dev), raid_disk);
				continue;
			}
			printk(KERN_INFO "raid1: device %s operational as mirror %d\n", kdevname(realdev->dev), raid_disk);
			raid_conf->mirrors[raid_disk].number = descriptor->number;
			raid_conf->mirrors[raid_disk].raid_disk = raid_disk;
			raid_conf->mirrors[raid_disk].dev = mddev->devices [i].dev;
			raid_conf->mirrors[raid_disk].operational = 1;
			raid_conf->mirrors[raid_disk].sect_limit = 128;
			raid_conf->working_disks++;
		}
	}
	if (!raid_conf->working_disks) {
		printk(KERN_ERR "raid1: no operational mirrors for %s\n", kdevname(MKDEV(MD_MAJOR, minor)));
		kfree(raid_conf);
		mddev->private = NULL;
		MOD_DEC_USE_COUNT;
		return -EIO;
	}

	raid_conf->raid_disks = sb->raid_disks;
	raid_conf->mddev = mddev;

	for (j = 0; !raid_conf->mirrors[j].operational; j++);
	raid_conf->last_used = j;
	for (i = raid_conf->raid_disks - 1; i >= 0; i--) {
		if (raid_conf->mirrors[i].operational) {
			PRINTK(("raid_conf->mirrors[%d].next == %d\n", i, j));
			raid_conf->mirrors[i].next = j;
			j = i;
		}
	}

	if (check_consistenty(mddev)) {
		printk(KERN_ERR "raid1: detected mirror differences -- run ckraid\n");
		sb->state |= 1 << MD_SB_ERRORS;
		kfree(raid_conf);
		mddev->private = NULL;
		MOD_DEC_USE_COUNT;
		return -EIO;
	}

	/*
	 * Regenerate the "device is in sync with the raid set" bit for
	 * each device.
	 */
	for (i = 0; i < sb->nr_disks ; i++) {
		sb->disks[i].state &= ~(1 << MD_SYNC_DEVICE);
		for (j = 0; j < sb->raid_disks; j++) {
			if (!raid_conf->mirrors[j].operational)
				continue;
			if (sb->disks[i].number == raid_conf->mirrors[j].number)
				sb->disks[i].state |= 1 << MD_SYNC_DEVICE;
		}
	}
	sb->active_disks = raid_conf->working_disks;

	printk("raid1: raid set %s active with %d out of %d mirrors\n", kdevname(MKDEV(MD_MAJOR, minor)), sb->active_disks, sb->raid_disks);
	/* Ok, everything is just fine now */
	return (0);
}

static int raid1_stop (int minor, struct md_dev *mddev)
{
	struct raid1_data *raid_conf = (struct raid1_data *) mddev->private;

	kfree (raid_conf);
	mddev->private = NULL;
	MOD_DEC_USE_COUNT;
	return 0;
}

static struct md_personality raid1_personality=
{
	"raid1",
	raid1_map,
	raid1_make_request,
	raid1_end_request,
	raid1_run,
	raid1_stop,
	raid1_status,
	NULL,			/* no ioctls */
	0,
	raid1_error
};

int raid1_init (void)
{
	if ((raid1_thread = md_register_thread(raid1d, NULL)) == NULL)
		return -EBUSY;
	return register_md_personality (RAID1, &raid1_personality);
}

#ifdef MODULE
int init_module (void)
{
	return raid1_init();
}

void cleanup_module (void)
{
	md_unregister_thread (raid1_thread);
	unregister_md_personality (RAID1);
}
#endif
