/*
 * SYSCALL_DEFINE2(setreuid16, old_uid_t, ruid, old_uid_t, euid)
 */
#include "sanitise.h"

struct syscall syscall_setreuid16 = {
	.name = "setreuid16",
	.num_args = 2,
	.arg1name = "ruid",
	.arg2name = "euid",
};
