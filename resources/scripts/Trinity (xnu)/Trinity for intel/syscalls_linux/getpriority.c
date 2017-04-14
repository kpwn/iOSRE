/*
 * SYSCALL_DEFINE2(getpriority, int, which, int, who)
 */

#include <sys/time.h>
#include <sys/resource.h>
#include "sanitise.h"

struct syscall syscall_getpriority = {
	.name = "getpriority",
	.num_args = 2,
	.arg1name = "which",
	.arg1type = ARG_OP,
	.arg1list = {
		.num = 3,
		.values = { PRIO_PROCESS, PRIO_PGRP, PRIO_USER },
	},
	.arg2name = "who",
	.arg2type = ARG_PID,
};
