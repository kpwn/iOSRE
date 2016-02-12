/*
 * SYSCALL_DEFINE1(oldumount, char __user *, name)
 */
#include "sanitise.h"

struct syscall syscall_oldumount = {
	.name = "oldumount",
	.num_args = 1,
	.arg1name = "name",
	.arg1type = ARG_ADDRESS,
};
