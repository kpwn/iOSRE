/*
 * SYSCALL_DEFINE4(socketpair, int, family, int, type, int, protocol, int __user *, usockvec)
 */
#include "sanitise.h"

struct syscall syscall_socketpair = {
	.name = "socketpair",
	.num_args = 4,
	.arg1name = "family",
	.arg2name = "type",
	.arg3name = "protocol",
	.arg4name = "usockvec",
	.arg4type = ARG_ADDRESS,
};
