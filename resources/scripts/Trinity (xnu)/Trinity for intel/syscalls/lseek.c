/*
 * SYSCALL_DEFINE3(lseek, unsigned int, fd, off_t, offset, unsigned int, origin)
 */
#include "sanitise.h"

struct syscall syscall_lseek = {
	.name = "lseek",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "offset",
	.arg3name = "origin",
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
