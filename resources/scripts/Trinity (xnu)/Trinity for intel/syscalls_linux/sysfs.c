/*
 * SYSCALL_DEFINE3(sysfs, int, option, unsigned long, arg1, unsigned long, arg2)
 */
#include "sanitise.h"

struct syscall syscall_sysfs = {
	.name = "sysfs",
	.num_args = 3,
	.arg1name = "option",
	.arg2name = "arg1",
	.arg3name = "arg2",
};
