/*
 * SYSCALL_DEFINE3(mknod, const char __user *, filename, int, mode, unsigned, dev)
 */
#include "sanitise.h"

struct syscall syscall_mknod = {
	.name = "mknod",
	.num_args = 3,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "mode",
	.arg2type = ARG_MODE_T,
	.arg3name = "dev",
	.group = GROUP_VFS,
};
