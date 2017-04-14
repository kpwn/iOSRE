/*
 * SYSCALL_DEFINE3(getresgid, gid_t __user *, rgid, gid_t __user *, egid, gid_t __user *, sgid)
 */
#include "sanitise.h"

struct syscall syscall_getresgid = {
	.name = "getresgid",
	.num_args = 3,
	.arg1name = "rgid",
	.arg1type = ARG_ADDRESS,
	.arg2name = "egid",
	.arg2type = ARG_ADDRESS,
	.arg3name = "sgid",
	.arg3type = ARG_ADDRESS,
};
