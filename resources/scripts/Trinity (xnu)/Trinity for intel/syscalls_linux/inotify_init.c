/*
 * SYSCALL_DEFINE0(inotify_init)
 */
#include "sanitise.h"

struct syscall syscall_inotify_init = {
	.name = "inotify_init",
	.num_args = 0,
	.group = GROUP_VFS,
};
