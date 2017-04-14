/*
 * SYSCALL_DEFINE4(mknodat, int, dfd, const char __user *, filename, int, mode, unsigned, dev)
 */
#include "sanitise.h"

struct syscall syscall_mknodat = {
	.name = "mknodat",
	.num_args = 4,
	.arg1name = "dfd",
	.arg1type = ARG_FD,
	.arg2name = "filename",
	.arg2type = ARG_PATHNAME,
	.arg3name = "mode",
	.arg4name = "dev",
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
