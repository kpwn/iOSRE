/*
 * SYSCALL_DEFINE2(lstat, const char __user *, filename,
                   struct __old_kernel_stat __user *, statbuf)
 */
#include "sanitise.h"

struct syscall syscall_lstat = {
	.name = "lstat",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "statbuf",
	.arg2type = ARG_ADDRESS,
	.group = GROUP_VFS,
};
