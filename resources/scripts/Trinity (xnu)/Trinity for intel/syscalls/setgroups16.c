/*
 * SYSCALL_DEFINE2(getgroups16, int, gidsetsize, old_gid_t __user *, grouplist)
 */
#include "sanitise.h"

struct syscall syscall_setgroups16 = {
	.name = "setgroups16",
	.num_args = 2,
	.arg1name = "gidsetsize",
	.arg2name = "grouplist",
	.arg2type = ARG_ADDRESS,
};
