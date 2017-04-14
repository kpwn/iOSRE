/*
 * SYSCALL_DEFINE2(sched_setparam, pid_t, pid, struct sched_param __user *, param)
 */
#include "sanitise.h"

struct syscall syscall_sched_setparam = {
	.name = "sched_setparam",
	.num_args = 2,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "param",
	.arg2type = ARG_ADDRESS,
};
