/*
 * SYSCALL_DEFINE1(sched_get_priority_min, int, policy)
 */
#include "sanitise.h"

struct syscall syscall_sched_get_priority_min = {
	.name = "sched_get_priority_min",
	.num_args = 1,
	.arg1name = "policy",
};
