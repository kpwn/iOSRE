/*
 * SYSCALL_DEFINE3(finit_module, int, fd, const char __user *, uargs, int, flags)
 */
#include "sanitise.h"

#define MODULE_INIT_IGNORE_MODVERSIONS  1
#define MODULE_INIT_IGNORE_VERMAGIC     2

struct syscall syscall_finit_module = {
	.name = "finit_module",
	.num_args = 3,
	.flags = CAPABILITY_CHECK,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "uargs",
	.arg2type = ARG_ADDRESS,
	.arg3name = "flags",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 2,
		.values = { MODULE_INIT_IGNORE_MODVERSIONS, MODULE_INIT_IGNORE_VERMAGIC },
	},
};
