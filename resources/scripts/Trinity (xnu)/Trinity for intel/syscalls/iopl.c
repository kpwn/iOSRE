/*
   long sys_iopl(unsigned int level, struct pt_regs *regs)
 */
#include "sanitise.h"

struct syscall syscall_iopl = {
	.name = "iopl",
	.num_args = 2,
	.arg1name = "level",
	.arg2name = "regs",
	.arg2type = ARG_ADDRESS,
};
