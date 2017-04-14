/*
 * SYSCALL_DEFINE1(setfsuid, uid_t, uid)
 */
#include "sanitise.h"

struct syscall syscall_setfsuid = {
	.name = "setfsuid",
	.num_args = 1,
	.arg1name = "uid",
	.group = GROUP_VFS,
};
