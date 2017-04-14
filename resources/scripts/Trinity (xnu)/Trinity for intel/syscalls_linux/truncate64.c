/*
 * SYSCALL_DEFINE(truncate64)(const char __user * path, loff_t length)
 */
#include "sanitise.h"

struct syscall syscall_truncate64 = {
	.name = "truncate64",
	.num_args = 2,
	.arg1name = "path",
	.arg1type = ARG_PATHNAME,
	.arg2name = "length",
	.arg2type = ARG_LEN,
	.group = GROUP_VFS,
};
