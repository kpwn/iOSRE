/*
 * SYSCALL_DEFINE1(fdatasync, unsigned int, fd)
 *
 * On success, returns zero.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_fdatasync = {
	.name = "fdatasync",
	.num_args = 1,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
