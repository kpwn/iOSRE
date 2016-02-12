/*
 * SYSCALL_DEFINE0(sched_yield)
 */
#include "sanitise.h"

struct syscall syscall_sched_yield = {
	.name = "sched_yield",
	.num_args = 0,
};
