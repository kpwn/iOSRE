/*
 * SYSCALL_DEFINE1(uselib, const char __user *, library)
 */
#include "sanitise.h"

struct syscall syscall_uselib = {
	.name = "uselib",
	.num_args = 1,
	.arg1name = "library",
	.arg1type = ARG_ADDRESS,
};
