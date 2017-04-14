/*
 * SYSCALL_DEFINE3(inotify_add_watch, int, fd, const char __user *, pathname, u32, mask)
 */
#include "sanitise.h"
#include "compat.h"

#include <sys/inotify.h>

struct syscall syscall_inotify_add_watch = {
	.name = "inotify_add_watch",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "pathname",
	.arg2type = ARG_PATHNAME,
	.arg3name = "mask",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 21,
		.values = {
			IN_ACCESS,
			IN_MODIFY,
			IN_ATTRIB,
			IN_CLOSE_WRITE,
			IN_CLOSE_NOWRITE,
			IN_OPEN,
			IN_MOVED_FROM,
			IN_MOVED_TO,
			IN_CREATE,
			IN_DELETE,
			IN_DELETE_SELF,
			IN_MOVE_SELF,
			IN_UNMOUNT,
			IN_Q_OVERFLOW,
			IN_IGNORED,
			IN_ONLYDIR,
			IN_DONT_FOLLOW,
			IN_EXCL_UNLINK,
			IN_MASK_ADD,
			IN_ISDIR,
			IN_ONESHOT,
		},
	},
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
