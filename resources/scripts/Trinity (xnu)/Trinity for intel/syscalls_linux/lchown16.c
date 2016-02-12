/*
 * SYSCALL_DEFINE3(lchown16, const char __user *, filename, old_uid_t, user, old_gid_t, group)
 */
#include "sanitise.h"

struct syscall syscall_lchown16 = {
	.name = "lchown16",
	.num_args = 3,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "user",
	.arg3name = "group",
	.group = GROUP_VFS,
};
