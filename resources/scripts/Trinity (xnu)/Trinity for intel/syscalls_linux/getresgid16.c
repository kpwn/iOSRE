/*
 * SYSCALL_DEFINE3(getresgid16, old_gid_t __user *, rgid, old_gid_t __user *, egid, old_gid_t __user *, sgid)
 */
#include "sanitise.h"

struct syscall syscall_getresgid16 = {
	.name = "getresgid16",
	.num_args = 3,
	.arg1name = "rgid",
	.arg1type = ARG_ADDRESS,
	.arg2name = "egid",
	.arg2type = ARG_ADDRESS,
	.arg3name = "sgid",
	.arg3type = ARG_ADDRESS,
};
