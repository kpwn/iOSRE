/*
 * SYSCALL_DEFINE5(fchownat, int, dfd, const char __user *, filename, uid_t, user,
	gid_t, group, int, flag)
 *
 * On success, fchownat() returns 0.
 *  On error, -1 is returned and errno is set to indicate the error.
 */
#include "sanitise.h"

struct syscall syscall_fchownat = {
	.name = "fchownat",
	.num_args = 5,
	.arg1name = "dfd",
	.arg1type = ARG_FD,
	.arg2name = "filename",
	.arg2type = ARG_PATHNAME,
	.arg3name = "user",
	.arg4name = "group",
	.arg5name = "flag",
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
