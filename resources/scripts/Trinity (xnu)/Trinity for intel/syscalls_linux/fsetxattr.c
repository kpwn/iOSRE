/*
 * SYSCALL_DEFINE5(fsetxattr, int, fd, const char __user *, name,
	 const void __user *,value, size_t, size, int, flags)
 */

#include <linux/xattr.h>
#include "sanitise.h"

struct syscall syscall_fsetxattr = {
	.name = "fsetxattr",
	.num_args = 5,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "name",
	.arg2type = ARG_ADDRESS,
	.arg3name = "value",
	.arg3type = ARG_ADDRESS,
	.arg4name = "size",
	.arg4type = ARG_LEN,
	.arg5name = "flags",
	.arg5type = ARG_LIST,
	.arg5list = {
		.num = 2,
		.values = { XATTR_CREATE , XATTR_REPLACE },
	},
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
