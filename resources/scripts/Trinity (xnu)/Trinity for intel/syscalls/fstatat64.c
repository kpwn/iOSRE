/*
 * SYSCALL_DEFINE4(fstatat64, int, dfd, const char __user *, filename,
	struct stat64 __user *, statbuf, int, flag)
 *
 * On success, fstatat() returns 0.
 * On error, -1 is returned and errno is set to indicate the error.
 */
#include "sanitise.h"

struct syscall syscall_fstatat64 = {
	.name = "fstatat64",
	.num_args = 4,
	.arg1name = "dfd",
	.arg1type = ARG_FD,
	.arg2name = "filename",
	.arg2type = ARG_PATHNAME,
	.arg3name = "statbuf",
	.arg3type = ARG_ADDRESS,
	.arg4name = "flag",
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
