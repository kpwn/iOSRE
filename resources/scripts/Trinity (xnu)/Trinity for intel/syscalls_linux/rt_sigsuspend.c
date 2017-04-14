/*
 * SYSCALL_DEFINE2(rt_sigsuspend, sigset_t __user *, unewset, size_t, sigsetsize)
 */
#include "sanitise.h"

struct syscall syscall_rt_sigsuspend = {
	.name = "rt_sigsuspend",
	.num_args = 2,
	.arg1name = "unewset",
	.arg1type = ARG_ADDRESS,
	.arg2name = "sigsetsize",
	.flags = NEED_ALARM,
};
