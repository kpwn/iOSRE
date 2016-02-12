/*
 * SYSCALL_DEFINE3(old_readdir, unsigned int, fd,
                 struct old_linux_dirent __user *, dirent, unsigned int, count)
 */
#include "sanitise.h"

struct syscall syscall_oldreaddir = {
	.name = "old_readdir",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "dirent",
	.arg2type = ARG_ADDRESS,
	.arg3name = "count",
	.arg3type = ARG_LEN,
};
