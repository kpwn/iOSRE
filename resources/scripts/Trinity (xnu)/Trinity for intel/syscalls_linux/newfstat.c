/*
 * SYSCALL_DEFINE2(newfstat, unsigned int, fd, struct stat __user *, statbuf)
 */
#include "sanitise.h"

struct syscall syscall_newfstat = {
	.name = "newfstat",
	.num_args = 2,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "statbuf",
	.arg2type = ARG_ADDRESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
