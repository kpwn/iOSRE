/*
 * SYSCALL_DEFINE1(set_tid_address, int __user *, tidptr)
 */
#include "sanitise.h"

struct syscall syscall_set_tid_address = {
	.name = "set_tid_address",
	.num_args = 1,
	.arg1name = "tidptr",
	.arg1type = ARG_ADDRESS,
	.flags = AVOID_SYSCALL,
};
