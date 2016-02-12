/*
 * SYSCALL_DEFINE2(ioprio_get, int, which, int, who)
 */
#include "sanitise.h"

struct syscall syscall_ioprio_get = {
	.name = "ioprio_get",
	.num_args = 2,
	.arg1name = "which",
	.arg2name = "who",
};
