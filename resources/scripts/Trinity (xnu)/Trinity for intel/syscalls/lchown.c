/*
 * SYSCALL_DEFINE3(lchown, const char __user *, filename, uid_t, user, gid_t, group)
 */
#include "sanitise.h"

struct syscall syscall_lchown = {
	.name = "lchown",
	.num_args = 3,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "user",
	.arg3name = "group",
	.group = GROUP_VFS,
};
