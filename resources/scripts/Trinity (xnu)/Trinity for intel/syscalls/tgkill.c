/*
 * SYSCALL_DEFINE3(tgkill, pid_t, tgid, pid_t, pid, int, sig)
 */
#include "sanitise.h"

struct syscall syscall_tgkill = {
	.name = "tgkill",
	.num_args = 3,
	.arg1name = "tgid",
	.arg1type = ARG_PID,
	.arg2name = "pid",
	.arg2type = ARG_PID,
	.arg3name = "sig",
	.flags = AVOID_SYSCALL,
};
