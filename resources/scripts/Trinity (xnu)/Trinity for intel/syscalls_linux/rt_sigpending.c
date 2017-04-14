/*
 * SYSCALL_DEFINE2(rt_sigpending, sigset_t __user *, set, size_t, sigsetsize)
 */
#include "sanitise.h"

struct syscall syscall_rt_sigpending = {
	.name = "rt_sigpending",
	.num_args = 2,
	.arg1name = "set",
	.arg1type = ARG_ADDRESS,
	.arg2name = "sigsetsize",
};
