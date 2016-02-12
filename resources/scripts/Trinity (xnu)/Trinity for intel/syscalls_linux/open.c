/*
 * SYSCALL_DEFINE3(open, const char __user *, filename, int, flags, int, mode)
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_open = {
	.name = "open",
	.num_args = 3,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "flags",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 20,
		.values = { O_RDONLY, O_WRONLY, O_RDWR,
				O_CREAT, O_EXCL, O_NOCTTY,
				O_TRUNC, O_APPEND, O_NONBLOCK,
				O_SYNC, O_ASYNC,
				O_DIRECTORY, O_NOFOLLOW, O_CLOEXEC,
				O_DIRECT, O_NOATIME, O_PATH,
				O_DSYNC, O_LARGEFILE, O_TMPFILE },
	},
	.arg3name = "mode",
	.arg3type = ARG_MODE_T,
};
