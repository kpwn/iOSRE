/*
 * SYSCALL_DEFINE3(init_module, void __user *, umod,
	 unsigned long, len, const char __user *, uargs)
 */
#include "sanitise.h"

struct syscall syscall_init_module = {
	.name = "init_module",
	.num_args = 3,
	.flags = CAPABILITY_CHECK,
	.arg1name = "umod",
	.arg1type = ARG_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.arg3name = "uargs",
	.arg3type = ARG_ADDRESS,
};
