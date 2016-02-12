/*
 * SYSCALL_DEFINE3(getdents, unsigned int, fd,
    struct linux_dirent __user *, dirent, unsigned int, count)
 */
#include "sanitise.h"

struct syscall syscall_getdents = {
	.name = "getdents",
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
