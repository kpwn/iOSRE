/*
 * asmlinkage int
   sys_sigaction(int sig, const struct old_sigaction __user *act,
   struct old_sigaction __user *oact)
 */
#include "sanitise.h"

struct syscall syscall_sigaction = {
	.name = "sigaction",
	.num_args = 3,
	.sanitise = sanitise_rt_sigaction,
	.arg1name = "sig",
	.arg2name = "act",
	.arg2type = ARG_ADDRESS,
	.arg3name = "oact",
	.flags = AVOID_SYSCALL,
};
