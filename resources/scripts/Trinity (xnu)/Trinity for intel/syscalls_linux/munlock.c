/*
 * SYSCALL_DEFINE2(munlock, unsigned long, start, size_t, len)
 */
#include "sanitise.h"

struct syscall syscall_munlock = {
	.name = "munlock",
	.num_args = 2,
	.arg1name = "addr",
	.arg1type = ARG_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.group = GROUP_VM,
};
