/*
 * SYSCALL_DEFINE3(setitimer, int, which, struct itimerval __user *, value, struct itimerval __user *, ovalue)
 */
#include "sanitise.h"

struct syscall syscall_setitimer = {
	.flags = AVOID_SYSCALL,		/* setitimer interferes with alarm() */
	.name = "setitimer",
	.num_args = 3,
	.arg1name = "which",
	.arg2name = "value",
	.arg2type = ARG_ADDRESS,
	.arg3name = "ovalue",
	.arg3type = ARG_ADDRESS,
};
