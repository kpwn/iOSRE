/*
 * SYSCALL_DEFINE2(truncate, const char __user *, path, long, length)
 */
#include "sanitise.h"

struct syscall syscall_truncate = {
	.name = "truncate",
	.num_args = 2,
	.arg1name = "path",
	.arg1type = ARG_PATHNAME,
	.arg2name = "length",
	.arg2type = ARG_LEN,
	.group = GROUP_VFS,
};
