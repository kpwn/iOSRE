/*
 * SYSCALL_DEFINE4(renameat, int, olddfd, const char __user *, oldname,
	 int, newdfd, const char __user *, newname)
 */
#include "sanitise.h"

struct syscall syscall_renameat = {
	.name = "renameat",
	.num_args = 4,
	.arg1name = "olddfd",
	.arg1type = ARG_FD,
	.arg2name = "oldname",
	.arg2type = ARG_ADDRESS,
	.arg3name = "newdfd",
	.arg3type = ARG_FD,
	.arg4name = "newname",
	.arg4type = ARG_ADDRESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
