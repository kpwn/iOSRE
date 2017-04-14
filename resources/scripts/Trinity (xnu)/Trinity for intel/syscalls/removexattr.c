/*
 * SYSCALL_DEFINE2(removexattr, const char __user *, pathname, const char __user *, name)
 */
#include "sanitise.h"

struct syscall syscall_removexattr = {
	.name = "removexattr",
	.num_args = 2,
	.arg1name = "pathname",
	.arg1type = ARG_PATHNAME,
	.arg2name = "name",
	.arg2type = ARG_ADDRESS,
};
