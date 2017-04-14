/*
 * SYSCALL_DEFINE0(getuid)
 */
#include "sanitise.h"

struct syscall syscall_getuid = {
	.name = "getuid",
	.num_args = 0,
};
