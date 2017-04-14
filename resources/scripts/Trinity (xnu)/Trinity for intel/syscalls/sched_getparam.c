/*
 * SYSCALL_DEFINE2(sched_getparam, pid_t, pid, struct sched_param __user *, param)
 */
#include "sanitise.h"

struct syscall syscall_sched_getparam = {
	.name = "sched_getparam",
	.num_args = 2,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "param",
	.arg2type = ARG_ADDRESS,
};
