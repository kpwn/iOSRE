/*
 * SYSCALL_DEFINE1(getpgid, pid_t, pid)
 */
#include "sanitise.h"

struct syscall syscall_getpgid = {
	.name = "getpgid",
	.num_args = 1,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.rettype = RET_PID_T,
};
