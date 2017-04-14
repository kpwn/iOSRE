/*
 * SYSCALL_DEFINE5(waitid, int, which, pid_t, upid, struct siginfo __user *,
	infop, int, options, struct rusage __user *, ru)
 */
#include "sanitise.h"

struct syscall syscall_waitid = {
	.name = "waitid",
	.num_args = 5,
	.arg1name = "which",
	.arg2name = "upid",
	.arg2type = ARG_PID,
	.arg3name = "infop",
	.arg3type = ARG_ADDRESS,
	.arg4name = "options",
	.arg5name = "ru",
	.arg5type = ARG_ADDRESS,
};
