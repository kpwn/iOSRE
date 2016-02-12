/*
   asmlinkage long sys_ioperm(unsigned long from, unsigned long num, int turn_on)
 */
#include "sanitise.h"

struct syscall syscall_ioperm = {
	.name = "ioperm",
	.num_args = 3,
	.arg1name = "from",
	.arg2name = "num",
	.arg3name = "turn_on",
};
