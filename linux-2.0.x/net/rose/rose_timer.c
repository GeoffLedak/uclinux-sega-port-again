/*
 *	ROSE release 003
 *
 *	This code REQUIRES 2.1.0 or higher/ NET3.029
 *
 *	This module:
 *		This module is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	History
 *	ROSE 001	Jonathan(G4KLX)	Cloned from nr_timer.c
 */

#include <linux/config.h>
#if defined(CONFIG_ROSE) || defined(CONFIG_ROSE_MODULE)
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <net/ax25.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <asm/segment.h>
#include <asm/system.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <net/rose.h>

static void rose_timer(unsigned long);

/*
 *	Linux set timer
 */
void rose_set_timer(struct sock *sk)
{
	unsigned long flags;

	save_flags(flags); cli();
	del_timer(&sk->timer);
	restore_flags(flags);

	sk->timer.data     = (unsigned long)sk;
	sk->timer.function = &rose_timer;
	sk->timer.expires  = jiffies + 10;

	add_timer(&sk->timer);
}

/*
 *	ROSE Timer
 *
 *	This routine is called every 100ms. Decrement timer by this
 *	amount - if expired then process the event.
 */
static void rose_timer(unsigned long param)
{
	struct sock *sk = (struct sock *)param;
	struct device *first;

	switch (sk->protinfo.rose->state) {
		case ROSE_STATE_0:
			/* Magic here: If we listen() and a new link dies before it
			   is accepted() it isn't 'dead' so doesn't get removed. */
			if (sk->destroy || (sk->state == TCP_LISTEN && sk->dead)) {
				del_timer(&sk->timer);
				rose_destroy_socket(sk);
				return;
			}
			break;

		case ROSE_STATE_3:
			/*
			 * Check for the state of the receive buffer.
			 */
			if (sk->rmem_alloc < ((sk->rcvbuf - ROSE_MAX_WINDOW_LEN) / 2) && (sk->protinfo.rose->condition & ROSE_COND_OWN_RX_BUSY)) {
				sk->protinfo.rose->condition &= ~ROSE_COND_OWN_RX_BUSY;
				sk->protinfo.rose->condition &= ~ROSE_COND_ACK_PENDING;
				sk->protinfo.rose->vl         = sk->protinfo.rose->vr;
				sk->protinfo.rose->timer      = 0;
				rose_write_internal(sk, ROSE_RR);
				break;
			}
			/*
			 * Check for frames to transmit.
			 */
			rose_kick(sk);
			break;

		default:
			break;
	}

	if (sk->protinfo.rose->timer == 0 || --sk->protinfo.rose->timer > 0) {
		rose_set_timer(sk);
		return;
	}

	/*
	 * Timer has expired, it may have been T1, T2, T3 or HB. We can tell
	 * by the socket state.
	 */
	switch (sk->protinfo.rose->state) {
		case ROSE_STATE_3:	/* HB */
			if (sk->protinfo.rose->condition & ROSE_COND_ACK_PENDING) {
				sk->protinfo.rose->condition &= ~ROSE_COND_ACK_PENDING;
				rose_enquiry_response(sk);
			}
			break;

		case ROSE_STATE_1:	/* T1 */
		case ROSE_STATE_4:	/* T2 */
			rose_write_internal(sk, ROSE_CLEAR_REQUEST);
			/* F6FBB - Disconnect the calling station
			sk->protinfo.rose->state = ROSE_STATE_2;
			sk->protinfo.rose->timer = sk->protinfo.rose->t3;
			break; */
			
			/* Falls to next case */

		case ROSE_STATE_2:	/* T3 */
			/* F6FBB - Added Cause and diagnostic */
			sk->protinfo.rose->cause = ROSE_DTE_ORIGINATED;
			sk->protinfo.rose->diagnostic = 0x30;
			
			/* F6FBB - Added Facilities */
			first = rose_dev_first();
			if (first) {	
				sk->protinfo.rose->facilities.fail_call = rose_callsign;
				memcpy(&sk->protinfo.rose->facilities.fail_addr, first->dev_addr, ROSE_ADDR_LEN);
			}
	
			rose_clear_queues(sk);
			sk->protinfo.rose->neighbour->use--;
			sk->protinfo.rose->state = ROSE_STATE_0;
			sk->state                = TCP_CLOSE;
			sk->err                  = ETIMEDOUT;
			sk->shutdown            |= SEND_SHUTDOWN;
			if (!sk->dead)
				sk->state_change(sk);
			sk->dead                 = 1;
			break;
	}

	rose_set_timer(sk);
}

#endif
