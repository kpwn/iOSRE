/*
 * SYSCALL_DEFINE3(set_mempolicy, int, mode, unsigned long __user *, nmask, unsigned long, maxnode)
 */
#include "sanitise.h"

struct syscall syscall_set_mempolicy = {
	.name = "set_mempolicy",
	.num_args = 3,
	.arg1name = "mode",
	.arg2name = "nmask",
	.arg2type = ARG_ADDRESS,
	.arg3name = "maxnode",
};
