/*
 * SYSCALL_DEFINE4(openat, int, dfd, const char __user *, filename, int, flags, int, mode)
 */
#include <fcntl.h>
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_openat = {
	.name = "openat",
	.num_args = 4,
	.arg1name = "dfd",
	.arg1type = ARG_FD,
	.arg2name = "filename",
	.arg2type = ARG_PATHNAME,
	.arg3name = "flags",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 20,
		.values = { O_RDONLY, O_WRONLY, O_RDWR,
				O_CREAT, O_EXCL, O_NOCTTY,
				O_TRUNC, O_APPEND, O_NONBLOCK,
				O_SYNC, O_ASYNC,
				O_DIRECTORY, O_NOFOLLOW, O_CLOEXEC,
				O_DIRECT, O_NOATIME, O_PATH,
				O_DSYNC, O_LARGEFILE, O_TMPFILE },
	},
	.arg4name = "mode",
	.arg4type = ARG_MODE_T,
	.flags = NEED_ALARM,
};
