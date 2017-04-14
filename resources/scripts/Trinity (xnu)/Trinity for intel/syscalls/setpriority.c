/*
 * SYSCALL_DEFINE3(setpriority, int, which, int, who, int, niceval)
 */
#include "sanitise.h"

struct syscall syscall_setpriority = {
	.name = "setpriority",
	.num_args = 3,
	.arg1name = "which",
	.arg2name = "who",
	.arg3name = "niceval",
};
