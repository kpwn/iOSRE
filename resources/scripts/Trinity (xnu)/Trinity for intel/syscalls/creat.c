/*
 * SYSCALL_DEFINE2(creat, const char __user *, pathname, int, mode)
 *
 * returns the new file descriptor on success.
 * returns -1 if an error occurred (in which case, errno is set appropriately).
 */
#include "sanitise.h"

struct syscall syscall_creat = {
	.name = "creat",
	.num_args = 2,
	.arg1name = "pathname",
	.arg1type = ARG_PATHNAME,
	.arg2name = "mode",
	.arg2type = ARG_MODE_T,
	.rettype = RET_FD,
	.group = GROUP_VFS,
};
