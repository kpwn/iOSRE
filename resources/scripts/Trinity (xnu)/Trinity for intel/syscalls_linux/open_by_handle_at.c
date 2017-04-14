/*
 * SYSCALL_DEFINE3(open_by_handle_at, int, mountdirfd,
 *               struct file_handle __user *, handle,
 *               int, flags)
 */
#include <fcntl.h>
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_open_by_handle_at = {
	.name = "open_by_handle_at",
	.num_args = 3,
	.arg1name = "mountdirfd",
	.arg1type = ARG_FD,
	.arg2name = "handle",
	.arg2type = ARG_ADDRESS,
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
	.flags = NEED_ALARM,
};
