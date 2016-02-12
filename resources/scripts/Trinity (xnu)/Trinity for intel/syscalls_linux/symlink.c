/*
 * SYSCALL_DEFINE2(symlink, const char __user *, oldname, const char __user *, newname)
 */
#include "sanitise.h"

struct syscall syscall_symlink = {
	.name = "symlink",
	.num_args = 2,
	.arg1name = "oldname",
	.arg1type = ARG_PATHNAME,
	.arg2name = "newname",
	.arg2type = ARG_PATHNAME,
	.group = GROUP_VFS,
};
