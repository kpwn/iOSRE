/*
   long sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss,
	 struct pt_regs *regs)
 */
#include "sanitise.h"

struct syscall syscall_sigaltstack = {
	.name = "sigaltstack",
	.num_args = 3,
	.arg1name = "uss",
	.arg1type = ARG_ADDRESS,
	.arg2name = "uoss",
	.arg2type = ARG_ADDRESS,
	.arg3name = "regs",
	.arg3type = ARG_ADDRESS,
};
