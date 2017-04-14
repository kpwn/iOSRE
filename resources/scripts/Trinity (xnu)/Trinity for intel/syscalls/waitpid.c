/*
 * SYSCALL_DEFINE3(waitpid, pid_t, pid, int __user *, stat_addr, int, options)
 */
#include "sanitise.h"

struct syscall syscall_waitpid = {
	.name = "waitpid",
	.num_args = 3,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "stat_addr",
	.arg2type = ARG_ADDRESS,
	.arg3name = "options",
};
