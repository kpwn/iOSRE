/*
 * SYSCALL_DEFINE3(llistxattr, const char __user *, pathname, char __user *, list, size_t, size)
 */
#include "sanitise.h"

struct syscall syscall_llistxattr = {
	.name = "llistxattr",
	.arg1name = "pathname",
	.arg1type = ARG_PATHNAME,
	.arg2name = "list",
	.arg2type = ARG_ADDRESS,
	.arg3name = "size",
	.arg3type = ARG_LEN,
	.num_args = 3,
	.group = GROUP_VFS,
};
