/*
 * SYSCALL_DEFINE0(getpid)
 */
#include "sanitise.h"

struct syscall syscall_getpid = {
	.name = "getpid",
	.num_args = 0,
	.rettype = RET_PID_T,
};
