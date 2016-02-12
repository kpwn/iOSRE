/*
 * SYSCALL_DEFINE(readahead)(int fd, loff_t offset, size_t count)
 */
#include "sanitise.h"

struct syscall syscall_readahead = {
	.name = "readahead",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "offset",
	.arg3name = "count",
	.arg3type = ARG_LEN,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
