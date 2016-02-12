/*
 * SYSCALL_DEFINE3(ioprio_set, int, which, int, who, int, ioprio)
 */
#include "sanitise.h"

struct syscall syscall_ioprio_set = {
	.name = "ioprio_set",
	.num_args = 3,
	.arg1name = "which",
	.arg2name = "who",
	.arg3name = "ioprio",
};
