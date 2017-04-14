/*
 * SYSCALL_DEFINE2(ustat, unsigned, dev, struct ustat __user *, ubuf)
 */
#include "sanitise.h"

struct syscall syscall_ustat = {
	.name = "ustat",
	.num_args = 2,
	.arg1name = "dev",
	.arg2name = "ubuf",
	.arg2type = ARG_ADDRESS,
	.group = GROUP_VFS,
};
