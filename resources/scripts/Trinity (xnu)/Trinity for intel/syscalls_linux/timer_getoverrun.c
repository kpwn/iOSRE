/*
 * SYSCALL_DEFINE1(timer_getoverrun, timer_t, timer_id)
 */
#include "sanitise.h"

struct syscall syscall_timer_getoverrun = {
	.name = "timer_getoverrun",
	.num_args = 1,
	.arg1name = "timer_id",
};
