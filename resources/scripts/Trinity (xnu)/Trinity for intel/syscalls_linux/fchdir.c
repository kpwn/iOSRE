/*
 * SYSCALL_DEFINE1(fchdir, unsigned int, fd)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_fchdir = {
	.name = "fchdir",
	.num_args = 1,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
