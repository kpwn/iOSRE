/*
 * SYSCALL_DEFINE2(chmod, const char __user *, filename, mode_t, mode)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_chmod = {
	.name = "chmod",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_PATHNAME,
	.arg2name = "mode",
	.arg2type = ARG_MODE_T,
	.rettype = RET_ZERO_SUCCESS,
};
