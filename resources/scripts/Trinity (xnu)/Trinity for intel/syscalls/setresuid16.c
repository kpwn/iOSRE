/*
 * SYSCALL_DEFINE3(setresuid16, old_uid_t, ruid, old_uid_t, euid, old_uid_t, suid)
 */
#include "sanitise.h"

struct syscall syscall_setresuid16 = {
	.name = "setresuid16",
	.num_args = 3,
	.arg1name = "ruid",
	.arg2name = "euid",
	.arg3name = "suid",
};
