/*
 * SYSCALL_DEFINE3(setresgid, gid_t, rgid, gid_t, egid, gid_t, sgid)
 */
#include "sanitise.h"

struct syscall syscall_setresgid = {
	.name = "setresgid",
	.num_args = 3,
	.arg1name = "rgid",
	.arg2name = "egid",
	.arg3name = "sgid",
};
