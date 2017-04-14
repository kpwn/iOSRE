/*
 * SYSCALL_DEFINE2(umount, char __user *, name, int, flags)
 */
#include <sys/mount.h>
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_umount = {
	.name = "umount",
	.num_args = 2,
	.arg1name = "name",
	.arg1type = ARG_PATHNAME,
	.arg2name = "flags",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 4,
		.values = { MNT_FORCE, MNT_DETACH, MNT_EXPIRE, UMOUNT_NOFOLLOW },
	},
	.group = GROUP_VFS,
};
