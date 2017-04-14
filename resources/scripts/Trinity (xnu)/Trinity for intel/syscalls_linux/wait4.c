/*
 * SYSCALL_DEFINE4(wait4, pid_t, upid, int __user *, stat_addr,
	 int, options, struct rusage __user *, ru)
 */
#include "sanitise.h"

struct syscall syscall_wait4 = {
	.name = "wait4",
	.num_args = 4,
	.arg1name = "upid",
	.arg1type = ARG_PID,
	.arg2name = "stat_addr",
	.arg2type = ARG_ADDRESS,
	.arg3name = "options",
	.arg4name = "ru",
	.arg4type = ARG_ADDRESS,
};
