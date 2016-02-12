/*
 * SYSCALL_DEFINE4(prlimit64, pid_t, pid, unsigned int, resource,
	 const struct rlimit64 __user *, new_rlim,
	 struct rlimit64 __user *, old_rlim)
 */
#include "sanitise.h"

struct syscall syscall_prlimit64 = {
	.name = "prlimit64",
	.num_args = 4,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "resource",
	.arg3name = "new_rlim",
	.arg3type = ARG_ADDRESS,
	.arg4name = "old_rlim",
	.arg4type = ARG_ADDRESS,
};
