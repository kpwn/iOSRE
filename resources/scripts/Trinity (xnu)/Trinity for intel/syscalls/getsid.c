/*
 * SYSCALL_DEFINE1(getsid, pid_t, pid)
 */
#include "sanitise.h"

struct syscall syscall_getsid = {
	.name = "getsid",
	.num_args = 1,
	.arg1name = "pid",
	.arg1type = ARG_PID,
};
