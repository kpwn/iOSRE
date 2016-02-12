/*
 * SYSCALL_DEFINE0(getgid)
 */
#include "sanitise.h"

struct syscall syscall_getgid16 = {
	.name = "getgid16",
	.num_args = 0,
	.rettype = RET_GID_T,
};
