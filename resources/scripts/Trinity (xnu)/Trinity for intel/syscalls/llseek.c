/*
 * SYSCALL_DEFINE5(llseek, unsigned int, fd, unsigned long, offset_high,
                unsigned long, offset_low, loff_t __user *, result,
                unsigned int, origin)
 */
#include "sanitise.h"

struct syscall syscall_llseek = {
	.name = "llseek",
	.num_args = 5,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "offset_high",
	.arg3name = "offset_low",
	.arg4name = "result",
	.arg5name = "origin",
	.group = GROUP_VFS,
};
