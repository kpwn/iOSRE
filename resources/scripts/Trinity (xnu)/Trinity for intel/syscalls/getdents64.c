/*
 * SYSCALL_DEFINE3(getdents64, unsigned int, fd,
	 struct linux_dirent64 __user *, dirent, unsigned int, count)
 */
#include "sanitise.h"

struct syscall syscall_getdents64 = {
	.name = "getdents64",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "dirent",
	.arg2type = ARG_ADDRESS,
	.arg3name = "count",
	.arg3type = ARG_LEN,
	.rettype = RET_NUM_BYTES,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
