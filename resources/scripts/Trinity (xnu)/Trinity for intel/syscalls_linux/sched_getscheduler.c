/*
 * SYSCALL_DEFINE1(sched_getscheduler, pid_t, pid)
 */
#include "sanitise.h"

struct syscall syscall_sched_getscheduler = {
	.name = "sched_getscheduler",
	.num_args = 1,
	.arg1name = "pid",
	.arg1type = ARG_PID,
};
