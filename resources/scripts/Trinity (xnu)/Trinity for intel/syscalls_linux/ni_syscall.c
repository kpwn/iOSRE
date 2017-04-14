/*
 */
#include "sanitise.h"

struct syscall syscall_ni_syscall = {
	.name = "ni_syscall (generic)",
	.num_args = 0,
	.flags = NI_SYSCALL,
};
