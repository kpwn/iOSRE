/*
 * SYSCALL_DEFINE2(signal, int, sig, __sighandler_t, handler)
 */
#include "sanitise.h"

struct syscall syscall_signal = {
	.name = "signal",
	.num_args = 2,
	.arg1name = "sig",
	.arg2name = "handler",
	.arg2type = ARG_ADDRESS,
	.flags = AVOID_SYSCALL,
};
