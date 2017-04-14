/*
 * SYSCALL_DEFINE3(getresuid16, old_uid_t __user *, ruid, old_uid_t __user *, euid, old_uid_t __user *, suid)
 */
#include "sanitise.h"

struct syscall syscall_getresuid16 = {
	.name = "getresuid16",
	.num_args = 3,
	.arg1name = "ruid",
	.arg1type = ARG_ADDRESS,
	.arg2name = "euid",
	.arg2type = ARG_ADDRESS,
	.arg3name = "suid",
	.arg3type = ARG_ADDRESS,
};
