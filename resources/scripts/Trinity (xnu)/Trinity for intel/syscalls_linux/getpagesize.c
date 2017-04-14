/*
 * sys_getpagesize (void)
 */
#include "sanitise.h"

struct syscall syscall_getpagesize = {
	.flags = BORING,
	.name = "getpagesize",
	.num_args = 0,
};
