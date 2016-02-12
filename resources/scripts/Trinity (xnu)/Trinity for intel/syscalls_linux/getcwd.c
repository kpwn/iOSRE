/*
 * SYSCALL_DEFINE2(getcwd, char __user *, buf, unsigned long, size)
 */
#include "sanitise.h"

struct syscall syscall_getcwd = {
	.name = "getcwd",
	.num_args = 2,
	.arg1name = "buf",
	.arg1type = ARG_ADDRESS,
	.arg2name = "size",
	.arg2type = ARG_LEN,
	.rettype = RET_PATH,
	.group = GROUP_VFS,
};
