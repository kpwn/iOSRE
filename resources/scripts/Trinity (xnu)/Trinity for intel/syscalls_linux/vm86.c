/*
 * int sys_vm86(unsigned long cmd, unsigned long arg, struct pt_regs *regs)
 */
#include "sanitise.h"

struct syscall syscall_vm86 = {
	.name = "vm86",
	.num_args = 3,
	.arg1name = "cmd",
	.arg2name = "arg",
	.arg3name = "regs",
	.arg3type = ARG_ADDRESS,
};
