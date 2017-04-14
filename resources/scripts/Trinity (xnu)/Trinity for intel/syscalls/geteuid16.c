/*
 * SYSCALL_DEFINE0(geteuid16)
 */
#include "sanitise.h"

struct syscall syscall_geteuid16 = {
	.name = "geteuid16",
	.num_args = 0,
	.rettype = RET_UID_T,
};
