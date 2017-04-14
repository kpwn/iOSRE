/*
 * SYSCALL_DEFINE2(tkill, pid_t, pid, int, sig)
 */
#include "sanitise.h"

struct syscall syscall_tkill = {
	.name = "tkill",
	.num_args = 2,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "sig",
	.flags = AVOID_SYSCALL,
};
