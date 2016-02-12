/*
 * SYSCALL_DEFINE2(pivot_root, const char __user *, new_root, const char __user *, put_old)
 */
#include "sanitise.h"

struct syscall syscall_pivot_root = {
	.name = "pivot_root",
	.num_args = 2,
	.flags = CAPABILITY_CHECK,
	.arg1name = "new_root",
	.arg1type = ARG_ADDRESS,
	.arg2name = "put_old",
	.arg2type = ARG_ADDRESS,
	.group = GROUP_VFS,
};
