/*
 * SYSCALL_DEFINE5(select, int, n, fd_set __user *, inp, fd_set __user *, outp,
	fd_set __user *, exp, struct timeval __user *, tvp)
 */
#include "sanitise.h"

struct syscall syscall_select = {
	.name = "select",
	.num_args = 5,
	.flags = AVOID_SYSCALL, // Confuses the signal state and causes the fuzzer to hang with timeout not firing
	.arg1name = "n",
	.arg2name = "inp",
	.arg2type = ARG_ADDRESS,
	.arg3name = "outp",
	.arg3type = ARG_ADDRESS,
	.arg4name = "exp",
	.arg4type = ARG_ADDRESS,
	.arg5name = "tvp",
	.arg5type = ARG_ADDRESS,
};
