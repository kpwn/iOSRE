/*
 * SYSCALL_DEFINE1(setfsuid16, old_uid_t, uid)
 */
#include "sanitise.h"

struct syscall syscall_setfsuid16 = {
	.name = "setfsuid16",
	.num_args = 1,
	.arg1name = "uid",
	.group = GROUP_VFS,
};
