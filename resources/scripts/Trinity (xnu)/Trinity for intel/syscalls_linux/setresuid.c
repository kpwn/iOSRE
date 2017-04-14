/*
 * SYSCALL_DEFINE3(setresuid, uid_t, ruid, uid_t, euid, uid_t, suid)
 */
#include "sanitise.h"

struct syscall syscall_setresuid = {
	.name = "setresuid",
	.num_args = 3,
	.arg1name = "ruid",
	.arg2name = "euid",
	.arg3name = "suid",
};
