/*
 * SYSCALL_DEFINE4(rt_tgsigqueueinfo, pid_t, tgid, pid_t, pid, int, sig,
	 siginfo_t __user *, uinfo)
 */
#include "sanitise.h"

struct syscall syscall_rt_tgsigqueueinfo = {
	.name = "rt_tgsigqueueinfo",
	.num_args = 4,
	.arg1name = "tgid",
	.arg2name = "pid",
	.arg2type = ARG_PID,
	.arg3name = "sig",
	.arg4name = "uinfo",
	.arg4type = ARG_ADDRESS,
};
