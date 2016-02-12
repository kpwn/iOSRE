/*
 * SYSCALL_DEFINE1(setgid16, old_gid_t, gid)
 */
#include "sanitise.h"

struct syscall syscall_setgid16 = {
	.name = "setgid16",
	.num_args = 1,
	.arg1name = "gid",
};
