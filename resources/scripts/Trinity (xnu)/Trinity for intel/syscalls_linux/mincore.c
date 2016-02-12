/*
 * SYSCALL_DEFINE3(mincore, unsigned long, start, size_t, len, unsigned char __user *, vec)
 */
#include "sanitise.h"

struct syscall syscall_mincore = {
	.name = "mincore",
	.num_args = 3,
	.arg1name = "start",
	.arg1type = ARG_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.arg3name = "vec",
	.arg3type = ARG_ADDRESS,
	.group = GROUP_VM,
};
