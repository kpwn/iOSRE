/*
 * SYSCALL_DEFINE2(rename, const char __user *, oldname, const char __user *, newname
 */
#include "sanitise.h"

struct syscall syscall_rename = {
	.name = "rename",
	.num_args = 2,
	.arg1name = "oldname",
	.arg1type = ARG_ADDRESS,
	.arg2name = "newname",
	.arg2type = ARG_ADDRESS,
	.group = GROUP_VFS,
};
