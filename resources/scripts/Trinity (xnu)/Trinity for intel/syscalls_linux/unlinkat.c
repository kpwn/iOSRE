/*
 * SYSCALL_DEFINE3(unlinkat, int, dfd, const char __user *, pathname, int, flag)
 */
#include "sanitise.h"

struct syscall syscall_unlinkat = {
	.name = "unlinkat",
	.num_args = 3,
	.arg1name = "dfd",
	.arg1type = ARG_FD,
	.arg2name = "pathname",
	.arg2type = ARG_PATHNAME,
	.arg3name = "flag",
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
