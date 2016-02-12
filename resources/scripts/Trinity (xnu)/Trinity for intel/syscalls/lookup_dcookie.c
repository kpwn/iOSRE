/*
 * SYSCALL_DEFINE(lookup_dcookie)(u64 cookie64, char __user * buf, size_t len)
 */
#include "sanitise.h"

struct syscall syscall_lookup_dcookie = {
	.name = "lookup_dcookie",
	.num_args = 3,
	.flags = CAPABILITY_CHECK,
	.arg1name = "cookie64",
	.arg2name = "buf",
	.arg2type = ARG_ADDRESS,
	.arg3name = "len",
	.arg3type = ARG_LEN,
	.group = GROUP_VFS,
};
