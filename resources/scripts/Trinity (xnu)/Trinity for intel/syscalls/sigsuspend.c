/*
 * asmlinkage int
   sys_sigsuspend(int history0, int history1, old_sigset_t mask)
 */
#include "sanitise.h"

struct syscall syscall_sigsuspend = {
	.name = "sigsuspend",
	.num_args = 2,
	.arg1name = "history0",
	.arg2name = "history1",
	.arg3name = "mask",
	.flags = AVOID_SYSCALL, // Confuses the signal state and causes the fuzzer to hang with timeout not firing
};
