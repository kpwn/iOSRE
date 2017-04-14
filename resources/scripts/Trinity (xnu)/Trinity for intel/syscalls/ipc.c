/*
 * SYSCALL_DEFINE6(ipc, unsigned int, call, int, first, unsigned long, second,
                  unsigned long, third, void __user *, ptr, long, fifth)
 */
#include "sanitise.h"

struct syscall syscall_ipc = {
	.name = "ipc",
	.num_args = 6,
	.arg1name = "call",
	.arg2name = "first",
	.arg3name = "second",
	.arg4name = "third",
	.arg5name = "ptr",
	.arg5type = ARG_ADDRESS,
	.arg6name = "fifth",
};
