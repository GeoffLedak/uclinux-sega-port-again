#ifndef __ARCH_I960_POSIX_TYPES_H
#define __ARCH_I960_POSIX_TYPES_H

/*
 * This file is generally used by user-level software, so you need to
 * be a little careful about namespace pollution etc.  Also, we cannot
 * assume GCC is being used.
 */

/* for i960, set all int's to short's, since m68k int is 2 bytes */
typedef unsigned short	__kernel_dev_t;
typedef unsigned long	__kernel_ino_t;
typedef unsigned short	__kernel_mode_t;
typedef unsigned short	__kernel_nlink_t;
typedef long		__kernel_off_t;
typedef short		__kernel_pid_t;
typedef unsigned short	__kernel_uid_t;
typedef unsigned short	__kernel_gid_t;
typedef unsigned long	__kernel_size_t;
typedef short		__kernel_ssize_t;
typedef short		__kernel_ptrdiff_t;
typedef long		__kernel_time_t;
typedef long		__kernel_clock_t;
typedef short		__kernel_daddr_t;
typedef char *		__kernel_caddr_t;

#ifdef __GNUC__
typedef long long	__kernel_loff_t;
#endif

typedef struct {
#if defined(__KERNEL__) || defined(__USE_ALL)
	int	val[2];
#else /* !defined(__KERNEL__) && !defined(__USE_ALL) */
	int	__val[2];
#endif /* !defined(__KERNEL__) && !defined(__USE_ALL) */
} __kernel_fsid_t;

#undef	__FD_SET
#define	__FD_SET(d, set)	((set)->fds_bits[__FDELT(d)] |= __FDMASK(d))

#undef	__FD_CLR
#define	__FD_CLR(d, set)	((set)->fds_bits[__FDELT(d)] &= ~__FDMASK(d))

#undef	__FD_ISSET
#define	__FD_ISSET(d, set)	((set)->fds_bits[__FDELT(d)] & __FDMASK(d))

#undef	__FD_ZERO
#define __FD_ZERO(fdsetp) (memset (fdsetp, 0, sizeof(*(fd_set *)fdsetp)))

#endif /* __ARCH_I960_POSIX_TYPES_H */
