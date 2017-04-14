/*
 * SYSCALL_DEFINE(fadvise64)(int fd, loff_t offset, size_t len, int advice)
 *
 * On success, zero is returned.
 * On error, an error number is returned.
 */
#include "sanitise.h"

struct syscall syscall_fadvise64 = {
	.name = "fadvise64",
	.num_args = 4,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "offset",
	.arg3name = "len",
	.arg3type = ARG_LEN,
	.arg4name = "advice",
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
