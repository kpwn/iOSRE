/*
 * SYSCALL_DEFINE1(sched_get_priority_max, int, policy)
 */
#include "sanitise.h"

struct syscall syscall_sched_get_priority_max = {
	.name = "sched_get_priority_max",
	.num_args = 1,
	.arg1name = "policy",
};
