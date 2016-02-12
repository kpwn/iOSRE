/*
 * SYSCALL_DEFINE0(geteuid)
 */
#include "sanitise.h"

struct syscall syscall_geteuid = {
	.name = "geteuid",
	.num_args = 0,
	.rettype = RET_UID_T,
};
