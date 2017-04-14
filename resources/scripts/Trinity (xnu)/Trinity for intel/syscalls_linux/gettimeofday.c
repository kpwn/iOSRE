/*
 * SYSCALL_DEFINE2(gettimeofday, struct timeval __user *, tv, struct timezone __user *, tz)
 */
#include "sanitise.h"

struct syscall syscall_gettimeofday = {
	.name = "gettimeofday",
	.num_args = 2,
	.arg1name = "tv",
	.arg1type = ARG_ADDRESS,
	.arg2name = "tz",
	.arg2type = ARG_ADDRESS,
};
