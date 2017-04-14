/*
 * SYSCALL_DEFINE2(inotify_rm_watch, int, fd, __s32, wd)
 */
#include "sanitise.h"

struct syscall syscall_inotify_rm_watch = {
	.name = "inotify_rm_watch",
	.num_args = 2,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "wd",
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
