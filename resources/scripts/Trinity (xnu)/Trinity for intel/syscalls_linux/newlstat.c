/*
 * SYSCALL_DEFINE2(newlstat, const char __user *, filename, struct stat __user *, statbuf)
 */
#include "sanitise.h"

struct syscall syscall_newlstat = {
	.name = "newlstat",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "statbuf",
	.arg2type = ARG_ADDRESS,
	.group = GROUP_VFS,
};
