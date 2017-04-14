/*
 * SYSCALL_DEFINE2(utimes, char __user *, filename, struct timeval __user *, utimes)
 */
#include "sanitise.h"

struct syscall syscall_utimes = {
	.name = "utimes",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "utimes",
	.arg2type = ARG_ADDRESS,
};
