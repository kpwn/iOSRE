/*
 * SYSCALL_DEFINE4(lgetxattr, const char __user *, pathname,
	 const char __user *, name, void __user *, value, size_t, size)
 */
#include "sanitise.h"

struct syscall syscall_lgetxattr = {
	.name = "lgetxattr",
	.num_args = 4,
	.arg1name = "pathname",
	.arg1type = ARG_PATHNAME,
	.arg2name = "name",
	.arg2type = ARG_ADDRESS,
	.arg3name = "value",
	.arg3type = ARG_ADDRESS,
	.arg4name = "size",
	.arg4type = ARG_LEN,
	.group = GROUP_VFS,
};
