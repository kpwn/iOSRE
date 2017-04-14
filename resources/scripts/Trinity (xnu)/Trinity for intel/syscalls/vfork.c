/*
   int sys_vfork(struct pt_regs *regs)
 */
#include "sanitise.h"

struct syscall syscall_vfork = {
	.name = "vfork",
	.num_args = 1,
	.flags = AVOID_SYSCALL, // No args, confuses fuzzer
	.arg1name = "regs",
};
