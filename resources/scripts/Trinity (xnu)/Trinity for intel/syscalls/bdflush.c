/*
 * SYSCALL_DEFINE2(bdflush, int, func, long, data)
 */
#include "sanitise.h"

struct syscall syscall_bdflush = {
	.name = "bdflush",
	.num_args = 2,
	.arg1name = "func",
	.arg2name = "data",
	.arg2type = ARG_ADDRESS,
};
