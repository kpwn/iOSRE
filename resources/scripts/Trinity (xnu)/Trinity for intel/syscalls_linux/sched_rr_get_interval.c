/*
 * SYSCALL_DEFINE2(sched_rr_get_interval, pid_t, pid, struct timespec __user *, interval)
 */
#include "sanitise.h"

struct syscall syscall_sched_rr_get_interval = {
	.name = "sched_rr_get_interval",
	.num_args = 2,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "interval",
	.arg2type = ARG_ADDRESS,
};
