/*
 * SYSCALL_DEFINE2(utime, char __user *, filename, struct utimbuf __user *, times)
 */
#include "sanitise.h"

struct syscall syscall_utime = {
	.name = "utime",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "times",
	.arg2type = ARG_ADDRESS,
};
