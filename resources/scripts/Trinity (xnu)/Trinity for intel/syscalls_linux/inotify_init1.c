/*
 * SYSCALL_DEFINE1(inotify_init1, int, flags)
 */

#define IN_CLOEXEC 02000000
#define IN_NONBLOCK 04000

#include "sanitise.h"

struct syscall syscall_inotify_init1 = {
	.name = "inotify_init1",
	.num_args = 1,
	.arg1name = "flags",
	.arg1type = ARG_LIST,
	.arg1list = {
		.num = 2,
		.values = { IN_CLOEXEC , IN_NONBLOCK },
	},
	.group = GROUP_VFS,
};
