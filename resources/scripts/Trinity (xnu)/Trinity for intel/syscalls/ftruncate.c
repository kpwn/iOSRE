/*
 * SYSCALL_DEFINE2(ftruncate, unsigned int, fd, unsigned long, length)
 */
#include "sanitise.h"

struct syscall syscall_ftruncate = {
	.name = "ftruncate",
	.num_args = 2,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "length",
	.arg2type = ARG_LEN,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
