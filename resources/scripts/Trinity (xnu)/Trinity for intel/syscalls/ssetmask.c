/*
 * SYSCALL_DEFINE1(ssetmask, int, newmask)
 */
#include "sanitise.h"

struct syscall syscall_ssetmask = {
	.name = "ssetmask",
	.num_args = 1,
	.arg1name = "newmask",
};
