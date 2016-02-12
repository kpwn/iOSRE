/*
 * SYSCALL_DEFINE2(delete_module, const char __user *, name_user, unsigned int, flags
 *
 * On success, zero is returned.
 * On error, -1 is returned and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_delete_module = {
	.name = "delete_module",
	.num_args = 2,
	.flags = CAPABILITY_CHECK,
	.arg1name = "name_user",
	.arg1type = ARG_ADDRESS,
	.arg2name = "flags",
	.rettype = RET_ZERO_SUCCESS,
};
