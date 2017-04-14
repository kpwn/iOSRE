/*
 * SYSCALL_DEFINE3(flistxattr, int, fd, char __user *, list, size_t, size)
 */
#include "sanitise.h"

struct syscall syscall_flistxattr = {
	.name = "flistxattr",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "list",
	.arg2type = ARG_ADDRESS,
	.arg3name = "size",
	.arg3type = ARG_LEN,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
