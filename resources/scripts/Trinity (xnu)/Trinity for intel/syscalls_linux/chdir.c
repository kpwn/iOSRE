/*
 * SYSCALL_DEFINE1(chdir, const char __user *, filename)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_chdir = {
	.name = "chdir",
	.num_args = 1,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.rettype = RET_ZERO_SUCCESS,
	.group = GROUP_VFS,
};
