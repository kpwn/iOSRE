/*
 * SYSCALL_DEFINE2(newstat, const char __user *, filename, struct stat __user *, statbuf)
 */
#include "sanitise.h"

struct syscall syscall_stat = {
	.name = "stat",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "statbuf",
	.arg2type = ARG_ADDRESS,
};
