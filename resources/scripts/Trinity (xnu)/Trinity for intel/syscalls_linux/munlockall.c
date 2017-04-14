/*
 * SYSCALL_DEFINE0(munlockall)
 */
#include "sanitise.h"

struct syscall syscall_munlockall = {
	.name = "munlockall",
	.num_args = 0,
	.group = GROUP_VM,
};
