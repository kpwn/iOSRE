/*
 * SYSCALL_DEFINE3(fchown16, unsigned int, fd, old_uid_t, user, old_gid_t, group)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_fchown16 = {
	.name = "fchown16",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "user",
	.arg3name = "group",
	.rettype = RET_ZERO_SUCCESS,
	.group = GROUP_VFS,
};
