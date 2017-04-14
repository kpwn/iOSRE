/*
 * SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, int, policy, struct sched_param __user *, param)
 */
#include "sanitise.h"

struct syscall syscall_sched_setscheduler = {
	.name = "sched_setscheduler",
	.num_args = 3,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "policy",
	.arg3name = "param",
	.arg3type = ARG_ADDRESS,
};
