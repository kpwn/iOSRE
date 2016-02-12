/*
 * SYSCALL_DEFINE6(pselect6, int, n, fd_set __user *, inp, fd_set __user *, outp,
	fd_set __user *, exp, struct timespec __user *, tsp,
	void __user *, sig)
 */
#include "sanitise.h"

struct syscall syscall_pselect6 = {
	.name = "pselect6",
	.num_args = 6,
	.flags = AVOID_SYSCALL, // Can cause the fuzzer to hang without timeout firing
	.arg1name = "n",
	.arg2name = "inp",
	.arg2type = ARG_ADDRESS,
	.arg3name = "outp",
	.arg3type = ARG_ADDRESS,
	.arg4name = "exp",
	.arg4type = ARG_ADDRESS,
	.arg5name = "tsp",
	.arg5type = ARG_ADDRESS,
	.arg6name = "sig",
	.arg6type = ARG_ADDRESS,
};
