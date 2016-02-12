/*
 * SYSCALL_DEFINE2(access, const char __user *, filename, int, mode)
 *
 * On  success  (all requested permissions granted), zero is returned.
 * On error (at least one bit in mode asked for a permission that is denied,
 *  or some other error occurred), -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_access = {
	.name = "access",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "mode",
	.arg2type = ARG_MODE_T,
	.group = GROUP_VFS,
};
