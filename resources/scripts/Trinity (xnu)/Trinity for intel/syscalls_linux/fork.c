/*
   int sys_fork(struct pt_regs *regs)
 */
#include "sanitise.h"

struct syscall syscall_fork = {
	.name = "fork",
	.num_args = 1,
	.flags = AVOID_SYSCALL, // No args to fuzz, confuses fuzzer
	.arg1name = "regs",
	.arg1type = ARG_ADDRESS,
};
