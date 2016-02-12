/*
 * SYSCALL_DEFINE1(fsync, unsigned int, fd)
 *
 * On success, these system calls return zero.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_fsync = {
	.name = "fsync",
	.num_args = 1,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
