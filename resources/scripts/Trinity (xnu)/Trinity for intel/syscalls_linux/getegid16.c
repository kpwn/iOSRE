/*
 * SYSCALL_DEFINE0(getegid16)
 */
#include "sanitise.h"

struct syscall syscall_getegid16 = {
	.name = "getegid16",
	.num_args = 0,
	.rettype = RET_GID_T,
};
