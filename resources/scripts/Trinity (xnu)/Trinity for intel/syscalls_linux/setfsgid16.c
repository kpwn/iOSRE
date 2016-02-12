/*
 * SYSCALL_DEFINE1(setfsgid16, old_gid_t, gid)
 */
#include "sanitise.h"

struct syscall syscall_setfsgid16 = {
	.name = "setfsgid16",
	.num_args = 1,
	.arg1name = "gid",
	.group = GROUP_VFS,
};
