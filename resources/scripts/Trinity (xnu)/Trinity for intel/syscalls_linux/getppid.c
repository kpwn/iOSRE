/*
 * SYSCALL_DEFINE0(getppid)
 */
#include "sanitise.h"

struct syscall syscall_getppid = {
	.name = "getppid",
	.num_args = 0,
	.rettype = RET_PID_T,
};
