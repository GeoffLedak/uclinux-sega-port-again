/*
 * Copyright (c) 2001 Hewlett-Packard, Matthew Wilcox
 *
 * $Id: syscallent.h,v 1.4 2001/08/03 12:27:54 wichert Exp $
 *
 */

	{ 5,	0,	printargs,		"SYS_0"			}, /* 0 */
	{ 1,	TP,	sys_exit,		"exit"			}, /* 1 */
	{ 0,	TP,	sys_fork,		"fork"			}, /* 2 */
	{ 3,	TF,	sys_read,		"read"			}, /* 3 */
	{ 3,	TF,	sys_write,		"write"			}, /* 4 */
	{ 3,	TF,	sys_open,		"open"			}, /* 5 */
	{ 1,	0,	sys_close,		"close"			}, /* 6 */
	{ 3,	TP,	sys_waitpid,		"waitpid"		}, /* 7 */
	{ 2,	TF,	sys_creat,		"creat"			}, /* 8 */
	{ 2,	TF,	sys_link,		"link"			}, /* 9 */
	{ 1,	TF,	sys_unlink,		"unlink"		}, /* 10 */
	{ 3,	TF|TP,	sys_execve,		"execve"		}, /* 11 */
	{ 1,	TF,	sys_chdir,		"chdir"			}, /* 12 */
	{ 1,	0,	sys_time,		"time"			}, /* 13 */
	{ 3,	TF,	sys_mknod,		"mknod"			}, /* 14 */
	{ 2,	TF,	sys_chmod,		"chmod"			}, /* 15 */
	{ 3,	TF,	printargs,		"lchown"		}, /* 16 */
	{ 3,	TN,	sys_socket,		"socket"		}, /* 17 */
	{ 2,	TF,	sys_stat,		"newstat"		}, /* 18 */
	{ 3,	TF,	sys_lseek,		"lseek"			}, /* 19 */
	{ 0,	0,	sys_getpid,		"getpid"		}, /* 20 */
	{ 5,	TF,	sys_mount,		"mount"			}, /* 21 */
	{ 3,	TN,	sys_bind,		"bind"			}, /* 22 */
	{ 1,	0,	sys_setuid,		"setuid"		}, /* 23 */
	{ 0,	0,	sys_getuid,		"getuid"		}, /* 24 */
	{ 1,	0,	sys_stime,		"stime"			}, /* 25 */
	{ 4,	0,	sys_ptrace,		"ptrace"		}, /* 26 */
	{ 1,	0,	sys_alarm,		"alarm"			}, /* 27 */
	{ 2,	0,	sys_fstat,		"newfstat"		}, /* 28 */
	{ 0,	0,	sys_pause,		"pause"			}, /* 29 */
	{ 2,	0,	sys_utime,		"utime"			}, /* 30 */
	{ 3,	TN,	sys_connect,		"connect"		}, /* 31 */
	{ 2,	TN,	sys_listen,		"listen"		}, /* 32 */
	{ 2,	TF,	sys_access,		"access"		}, /* 33 */
	{ 1,	0,	sys_nice,		"nice"			}, /* 34 */
	{ 3,	TN,	sys_accept,		"accept"		}, /* 35 */
	{ 0,	0,	sys_sync,		"sync"			}, /* 36 */
	{ 2,	TS,	sys_kill,		"kill"			}, /* 37 */
	{ 2,	TF,	sys_rename,		"rename"		}, /* 38 */
	{ 2,	TF,	sys_mkdir,		"mkdir"			}, /* 39 */
	{ 1,	TF,	sys_rmdir,		"rmdir"			}, /* 40 */
	{ 1,	0,	sys_dup,		"dup"			}, /* 41 */
	{ 1,	0,	sys_pipe,		"pipe"			}, /* 42 */
	{ 1,	0,	sys_times,		"times"			}, /* 43 */
	{ 3,	TN,	sys_getsockname,	"getsockname"		}, /* 44 */
	{ 1,	0,	sys_brk,		"brk"			}, /* 45 */
	{ 1,	0,	sys_setgid,		"setgid"		}, /* 46 */
	{ 0,	0,	sys_getgid,		"getgid"		}, /* 47 */
	{ 2,	0,	sys_signal,		"signal"		}, /* 48 */
	{ 0,	0,	sys_geteuid,		"geteuid"		}, /* 49 */
	{ 0,	0,	sys_getegid,		"getegid"		}, /* 50 */
	{ 1,	TF,	sys_acct,		"acct"			}, /* 51 */
	{ 2,	0,	sys_umount2,		"umount2"		}, /* 52 */
	{ 3,	TN,	sys_getpeername,	"lock"			}, /* 53 */
	{ 3,	0,	sys_ioctl,		"ioctl"			}, /* 54 */
	{ 3,	0,	sys_fcntl,		"fcntl"			}, /* 55 */
	{ 4,	TN,	sys_socketpair,		"socketpair"		}, /* 56 */
	{ 2,	0,	sys_setpgid,		"setpgid"		}, /* 57 */
	{ 4,	TN,	sys_send,		"send"			}, /* 58 */
	{ 1,	0,	sys_uname,		"newuname"			}, /* 59 */
	{ 1,	0,	sys_umask,		"umask"			}, /* 60 */
	{ 1,	TF,	sys_chroot,		"chroot"		}, /* 61 */
	{ 2,	0,	sys_ustat,		"ustat"			}, /* 62 */
	{ 2,	0,	sys_dup2,		"dup2"			}, /* 63 */
	{ 0,	0,	sys_getppid,		"getppid"		}, /* 64 */
	{ 0,	0,	sys_getpgrp,		"getpgrp"		}, /* 65 */
	{ 0,	0,	sys_setsid,		"setsid"		}, /* 66 */
	{ 5,	0,	printargs,		"pivot_root"		}, /* 67 */
	{ 5,	0,	printargs,		"sgetmask"		}, /* 68 */
	{ 5,	0,	printargs,		"ssetmask"		}, /* 69 */
	{ 2,	0,	sys_setreuid,		"setreuid"		}, /* 70 */
	{ 2,	0,	sys_setregid,		"setregid"		}, /* 71 */
	{ 5,	0,	printargs,		"mincore"		}, /* 72 */
	{ 1,	TS,	sys_sigpending,		"sigpending"		}, /* 73 */
	{ 2,	0,	sys_sethostname,	"sethostname"		}, /* 74 */
	{ 2,	0,	sys_setrlimit,		"setrlimit"		}, /* 75 */
	{ 2,	0,	sys_getrlimit,		"getrlimit"		}, /* 76 */
	{ 2,	0,	sys_getrusage,		"getrusage"		}, /* 77 */
	{ 2,	0,	sys_gettimeofday,	"gettimeofday"		}, /* 78 */
	{ 2,	0,	sys_settimeofday,	"settimeofday"		}, /* 79 */
	{ 2,	0,	sys_getgroups,		"getgroups"		}, /* 80 */
	{ 2,	0,	sys_setgroups,		"setgroups"		}, /* 81 */
	{ 6,	TN,	sys_sendto,		"sendto"		}, /* 82 */
	{ 2,	TF,	sys_symlink,		"symlink"		}, /* 83 */
	{ 2,	TF,	sys_lstat,		"newlstat"		}, /* 84 */
	{ 3,	TF,	sys_readlink,		"readlink"		}, /* 85 */
	{ 1,	0,	sys_uselib,		"uselib"		}, /* 86 */
	{ 1,	0,	sys_swapon,		"swapon"		}, /* 87 */
	{ 3,	0,	sys_reboot,		"reboot"		}, /* 88 */
	{ 6,	0,	sys_mmap,		"mmap2"			}, /* 89 */
	{ 6,	0,	sys_mmap,		"mmap"			}, /* 90 */
	{ 2,	0,	sys_munmap,		"munmap"		}, /* 91 */
	{ 2,	TF,	sys_truncate,		"truncate"		}, /* 92 */
	{ 2,	0,	sys_ftruncate,		"ftruncate"		}, /* 93 */
	{ 2,	0,	sys_fchmod,		"fchmod"		}, /* 94 */
	{ 3,	0,	sys_fchown,		"fchown"		}, /* 95 */
	{ 2,	0,	sys_getpriority,	"getpriority"		}, /* 96 */
	{ 3,	0,	sys_setpriority,	"setpriority"		}, /* 97 */
	{ 4,	TN,	sys_recv,		"recv"			}, /* 98 */
	{ 2,	TF,	sys_statfs,		"statfs"		}, /* 99 */
	{ 2,	0,	sys_fstatfs,		"fstatfs"		}, /* 100 */
	{ 2,	TF,	sys_stat64,		"stat64"		}, /* 101 */
	{ 5,	0,	printargs,		"SYS_102"		}, /* 102 */
	{ 3,	0,	sys_syslog,		"syslog"		}, /* 103 */
	{ 3,	0,	sys_setitimer,		"setitimer"		}, /* 104 */
	{ 2,	0,	sys_getitimer,		"getitimer"		}, /* 105 */
	{ 2,	0,	sys_capget,		"capget"		}, /* 106 */
	{ 2,	0,	sys_capset,		"capset"		}, /* 107 */
	{ 5,	TF,	sys_pread,		"pread"			}, /* 108 */
	{ 5,	TF,	sys_pwrite,		"pwrite"		}, /* 109 */
	{ 2,	0,	sys_getcwd,		"getcwd"		}, /* 110 */
	{ 0,	0,	sys_vhangup,		"vhangup"		}, /* 111 */
	{ 2,	TF,	sys_fstat64,		"fstat64"		}, /* 112 */
	{ 0,	0,	sys_vfork,		"vfork"			}, /* 113 */
	{ 4,	TP,	sys_wait4,		"wait4"			}, /* 114 */
	{ 1,	0,	sys_swapoff,		"swapoff"		}, /* 115 */
	{ 1,	0,	sys_sysinfo,		"sysinfo"		}, /* 116 */
	{ 2,	TN,	sys_shutdown,		"shutdown"		}, /* 117 */
	{ 1,	0,	sys_fsync,		"fsync"			}, /* 118 */
	{ 5,	0,	printargs,		"madvise"		}, /* 119 */
	{ 2,	TP,	sys_clone,		"clone"			}, /* 120 */
	{ 2,	0,	sys_setdomainname,	"setdomainname"		}, /* 121 */
	{ 4,	TF,	sys_sendfile,		"sendfile"		}, /* 122 */
	{ 6,	TN,	sys_recvfrom,		"recvfrom"		}, /* 123 */
	{ 1,	0,	sys_adjtimex,		"adjtimex"		}, /* 124 */
	{ 3,	0,	sys_mprotect,		"mprotect"		}, /* 125 */
	{ 3,	TS,	sys_sigprocmask,	"sigprocmask"		}, /* 126 */
	{ 2,	0,	sys_create_module,	"create_module"		}, /* 127 */
	{ 4,	0,	sys_init_module,	"init_module"		}, /* 128 */
	{ 1,	0,	sys_delete_module,	"delete_module"		}, /* 129 */
	{ 1,	0,	sys_get_kernel_syms,	"get_kernel_syms"	}, /* 130 */ 
	{ 4,	0,	sys_quotactl,		"quotactl"		}, /* 131 */
	{ 1,	0,	sys_getpgid,		"getpgid"		}, /* 132 */
	{ 1,	TF,	sys_fchdir,		"fchdir"		}, /* 133 */
	{ 0,	0,	sys_bdflush,		"bdflush"		}, /* 134 */
	{ 5,	0,	sys_sysfs,		"sysfs"			}, /* 135 */
	{ 1,	0,	sys_personality,	"personality"		}, /* 136 */
	{ 5,	0,	printargs,		"SYS_137"		}, /* 137 */
	{ 1,	0,	sys_setfsuid,		"setfsuid"		}, /* 138 */
	{ 1,	0,	sys_setfsgid,		"setfsgid"		}, /* 139 */
	{ 5,	TF,	printargs,		"_llseek"		}, /* 140 */
	{ 3,	0,	sys_getdents,		"getdents"		}, /* 141 */
	{ 5,	0,	printargs,		"_newselect"		}, /* 142 */
	{ 2,	0,	sys_flock,		"flock"			}, /* 143 */
	{ 3,	0,	sys_msync,		"msync"			}, /* 144 */
	{ 3,	0,	sys_readv,		"readv"			}, /* 145 */
	{ 3,	0,	sys_writev,		"writev"		}, /* 146 */
	{ 1,	0,	sys_getsid,		"getsid"		}, /* 147 */
	{ 1,	0,	sys_fdatasync,		"fdatasync"		}, /* 148 */
	{ 5,	0,	printargs,		"_sysctl"		}, /* 149 */
	{ 2,	0,	sys_mlock,		"mlock"			}, /* 150 */
	{ 2,	0,	sys_munlock,		"munlock"		}, /* 151 */
	{ 1,	0,	sys_mlockall,		"mlockall"		}, /* 152 */
	{ 1,	0,	sys_munlockall,		"munlockall"		}, /* 153 */
	{ 2,	0,	sys_sched_setparam,	"sched_setparam"	}, /* 154 */ 
	{ 2,	0,	sys_sched_getparam,	"sched_getparam"	}, /* 155 */ 
	{ 3,	0,	sys_sched_setscheduler,	"sched_setscheduler"	}, /* 156 */ 
	{ 2,	0,	sys_sched_getscheduler,	"sched_getscheduler"	}, /* 157 */ 
	{ 0,	0,	sys_sched_yield,	"sched_yield"		}, /* 158 */
	{ 1,	0,	sys_sched_get_priority_max,"sched_get_priority_max"}, /* 159 */
	{ 1,	0,	sys_sched_get_priority_min,"sched_get_priority_min"}, /* 160 */
	{ 2,	0,	sys_sched_rr_get_interval,"sched_rr_get_interval"}, /* 161 */
	{ 2,	0,	sys_nanosleep,		"nanosleep"		}, /* 162 */
	{ 4,	0,	sys_mremap,		"mremap"		}, /* 163 */
	{ 3,	0,	sys_setresuid,		"setresuid"		}, /* 164 */
	{ 3,	0,	sys_setresuid,		"getresuid"		}, /* 165 */
	{ 2,	TS,	sys_sigaltstack,	"sigaltstack"		}, /* 166 */
	{ 5,	0,	sys_query_module,	"query_module"		}, /* 167 */
	{ 3,	0,	sys_poll,		"poll"			}, /* 168 */
	{ 5,	0,	printargs,		"nfsservctl"		}, /* 169 */
	{ 3,	0,	sys_setresgid,		"setresgid"		}, /* 170 */
	{ 3,	0,	sys_setresgid,		"getresgid"		}, /* 171 */
	{ 5,	0,	sys_prctl,		"prctl"			}, /* 172 */
	{ 1,	TS,	printargs,		"rt_sigreturn"		}, /* 173 */
	{ 4,	TS,	sys_rt_sigaction,	"rt_sigaction"		}, /* 174 */
	{ 4,	TS,	sys_rt_sigprocmask,	"rt_sigprocmask"	}, /* 175 */ 
	{ 2,	TS,	sys_rt_sigpending,	"rt_sigpending"		}, /* 176 */
	{ 4,	TS,	sys_rt_sigtimedwait,	"rt_sigtimedwait"	}, /* 177 */ 
	{ 3,	TS,	sys_rt_sigqueueinfo,	"rt_sigqueueinfo"	}, /* 178 */ 
	{ 2,	TS,	sys_rt_sigsuspend,	"rt_sigsuspend"		}, /* 179 */
	{ 3,	TF,	sys_chown,		"chown"			}, /* 180 */
	{ 5,	TN,	sys_setsockopt,		"setsockopt"		}, /* 181 */
	{ 5,	TN,	sys_getsockopt,		"getsockopt"		}, /* 182 */
	{ 5,	TN,	sys_sendmsg,		"sendmsg"		}, /* 183 */
	{ 5,	TN,	sys_recvmsg,		"recvmsg"		}, /* 184 */
	{ 4,	TI,	sys_semop,		"semop"			}, /* 185 */
	{ 4,	TI,	sys_semget,		"semget"		}, /* 186 */
	{ 4,	TI,	sys_semctl,		"semctl"		}, /* 187 */
	{ 4,	TI,	sys_msgsnd,		"msgsnd"		}, /* 188 */
	{ 4,	TI,	sys_msgrcv,		"msgrcv"		}, /* 189 */
	{ 4,	TI,	sys_msgget,		"msgget"		}, /* 190 */
	{ 4,	TI,	sys_msgctl,		"msgctl"		}, /* 191 */
	{ 4,	TI,	sys_shmat,		"shmat"			}, /* 192 */
	{ 4,	TI,	sys_shmdt,		"shmdt"			}, /* 193 */
	{ 4,	TI,	sys_shmget,		"shmget"		}, /* 194 */
	{ 4,	TI,	sys_shmctl,		"shmctl"		}, /* 195 */

	{ 5,	0,	printargs,		"SYS_196"		}, /* 196 */
	{ 5,	0,	printargs,		"SYS_197"		}, /* 197 */
	{ 2,	TF,	sys_lstat64,		"lstat64"		}, /* 198 */
	{ 3,	TF,	printargs,		"truncate64"		}, /* 199 */
	{ 3,	TF,	printargs,		"ftruncate64"		}, /* 200 */
	{ 3,	0,	printargs,		"getdents64"		}, /* 201 */
	{ 3,	0,	sys_fcntl,		"fcntl64"		}, /* 202 */
	{ 5,	0,	printargs,		"SYS_203"		}, /* 203 */
	{ 5,	0,	printargs,		"SYS_204"		}, /* 204 */
	{ 5,	0,	printargs,		"SYS_205"		}, /* 205 */
	{ 5,	0,	printargs,		"SYS_206"		}, /* 206 */
	{ 5,	0,	printargs,		"SYS_207"		}, /* 207 */
	{ 5,	0,	printargs,		"SYS_208"		}, /* 208 */
	{ 5,	0,	printargs,		"SYS_209"		}, /* 209 */
	{ 5,	0,	printargs,		"SYS_210"		}, /* 210 */
	{ 5,	0,	printargs,		"SYS_211"		}, /* 211 */
	{ 5,	0,	printargs,		"SYS_212"		}, /* 212 */
	{ 5,	0,	printargs,		"SYS_213"		}, /* 213 */
	{ 5,	0,	printargs,		"SYS_214"		}, /* 214 */
	{ 5,	0,	printargs,		"SYS_215"		}, /* 215 */
	{ 5,	0,	printargs,		"SYS_216"		}, /* 216 */
	{ 5,	0,	printargs,		"SYS_217"		}, /* 217 */
	{ 5,	0,	printargs,		"SYS_218"		}, /* 218 */
	{ 5,	0,	printargs,		"SYS_219"		}, /* 219 */
	{ 5,	0,	printargs,		"SYS_220"		}, /* 220 */
	{ 5,	0,	printargs,		"SYS_221"		}, /* 221 */
	{ 5,	0,	printargs,		"SYS_222"		}, /* 222 */
	{ 5,	0,	printargs,		"SYS_223"		}, /* 223 */
	{ 5,	0,	printargs,		"SYS_224"		}, /* 224 */
	{ 5,	0,	printargs,		"SYS_225"		}, /* 225 */
	{ 5,	0,	printargs,		"SYS_226"		}, /* 226 */
	{ 5,	0,	printargs,		"SYS_227"		}, /* 227 */
	{ 5,	0,	printargs,		"SYS_228"		}, /* 228 */
	{ 5,	0,	printargs,		"SYS_229"		}, /* 229 */
	{ 5,	0,	printargs,		"SYS_230"		}, /* 230 */
	{ 5,	0,	printargs,		"SYS_231"		}, /* 231 */
	{ 5,	0,	printargs,		"SYS_232"		}, /* 232 */
	{ 5,	0,	printargs,		"SYS_233"		}, /* 233 */
	{ 5,	0,	printargs,		"SYS_234"		}, /* 234 */
	{ 5,	0,	printargs,		"SYS_235"		}, /* 235 */
	{ 5,	0,	printargs,		"SYS_236"		}, /* 236 */
	{ 5,	0,	printargs,		"SYS_237"		}, /* 237 */
	{ 5,	0,	printargs,		"SYS_238"		}, /* 238 */
	{ 5,	0,	printargs,		"SYS_239"		}, /* 239 */
	{ 5,	0,	printargs,		"SYS_240"		}, /* 240 */
	{ 5,	0,	printargs,		"SYS_241"		}, /* 241 */
	{ 5,	0,	printargs,		"SYS_242"		}, /* 242 */
	{ 5,	0,	printargs,		"SYS_243"		}, /* 243 */
	{ 5,	0,	printargs,		"SYS_244"		}, /* 244 */
	{ 5,	0,	printargs,		"SYS_245"		}, /* 245 */
	{ 5,	0,	printargs,		"SYS_246"		}, /* 246 */
	{ 5,	0,	printargs,		"SYS_247"		}, /* 247 */
	{ 5,	0,	printargs,		"SYS_248"		}, /* 248 */
	{ 5,	0,	printargs,		"SYS_249"		}, /* 249 */
	{ 5,	0,	printargs,		"SYS_250"		}, /* 250 */
	{ 5,	0,	printargs,		"SYS_251"		}, /* 251 */
	{ 5,	0,	printargs,		"SYS_252"		}, /* 252 */
	{ 5,	0,	printargs,		"SYS_253"		}, /* 253 */
	{ 5,	0,	printargs,		"SYS_254"		}, /* 254 */
	{ 5,	0,	printargs,		"SYS_255"		}, /* 255 */
	{ 5,	0,	printargs,		"SYS_256"		}, /* 256 */
	{ 5,	0,	printargs,		"SYS_257"		}, /* 257 */
	{ 5,	0,	printargs,		"SYS_258"		}, /* 258 */
	{ 5,	0,	printargs,		"SYS_259"		}, /* 259 */
	{ 5,	0,	printargs,		"SYS_260"		}, /* 260 */
	{ 5,	0,	printargs,		"SYS_261"		}, /* 261 */
	{ 5,	0,	printargs,		"SYS_262"		}, /* 262 */
	{ 5,	0,	printargs,		"SYS_263"		}, /* 263 */
	{ 5,	0,	printargs,		"SYS_264"		}, /* 264 */
	{ 5,	0,	printargs,		"SYS_265"		}, /* 265 */
	{ 5,	0,	printargs,		"SYS_266"		}, /* 266 */
	{ 5,	0,	printargs,		"SYS_267"		}, /* 267 */
	{ 5,	0,	printargs,		"SYS_268"		}, /* 268 */
	{ 5,	0,	printargs,		"SYS_269"		}, /* 269 */
	{ 5,	0,	printargs,		"SYS_270"		}, /* 270 */
	{ 5,	0,	printargs,		"SYS_271"		}, /* 271 */
	{ 5,	0,	printargs,		"SYS_272"		}, /* 272 */
	{ 5,	0,	printargs,		"SYS_273"		}, /* 273 */
	{ 5,	0,	printargs,		"SYS_274"		}, /* 274 */
	{ 5,	0,	printargs,		"SYS_275"		}, /* 275 */
	{ 5,	0,	printargs,		"SYS_276"		}, /* 276 */
	{ 5,	0,	printargs,		"SYS_277"		}, /* 277 */
	{ 5,	0,	printargs,		"SYS_278"		}, /* 278 */
	{ 5,	0,	printargs,		"SYS_279"		}, /* 279 */
	{ 5,	0,	printargs,		"SYS_280"		}, /* 280 */
	{ 5,	0,	printargs,		"SYS_281"		}, /* 281 */
	{ 5,	0,	printargs,		"SYS_282"		}, /* 282 */
	{ 5,	0,	printargs,		"SYS_283"		}, /* 283 */
	{ 5,	0,	printargs,		"SYS_284"		}, /* 284 */
	{ 5,	0,	printargs,		"SYS_285"		}, /* 285 */
	{ 5,	0,	printargs,		"SYS_286"		}, /* 286 */
	{ 5,	0,	printargs,		"SYS_287"		}, /* 287 */
	{ 5,	0,	printargs,		"SYS_288"		}, /* 288 */
	{ 5,	0,	printargs,		"SYS_289"		}, /* 289 */
	{ 5,	0,	printargs,		"SYS_290"		}, /* 290 */
	{ 5,	0,	printargs,		"SYS_291"		}, /* 291 */
	{ 5,	0,	printargs,		"SYS_292"		}, /* 292 */
	{ 5,	0,	printargs,		"SYS_293"		}, /* 293 */
	{ 5,	0,	printargs,		"SYS_294"		}, /* 294 */
