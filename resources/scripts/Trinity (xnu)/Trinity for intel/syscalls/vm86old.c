/*
 * int sys_vm86old(struct vm86_struct __user *v86, struct pt_regs *regs)
 */
#include "sanitise.h"

struct syscall syscall_vm86old = {
	.name = "vm86old",
	.num_args = 2,
	.arg1name = "v86",
	.arg1type = ARG_ADDRESS,
	.arg2name = "regs",
	.arg2type = ARG_ADDRESS,
};
