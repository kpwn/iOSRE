/*
 * SYSCALL_DEFINE3(timer_create, const clockid_t, which_clock,
	struct sigevent __user *, timer_event_spec,
	timer_t __user *, created_timer_id)
 */
#include "sanitise.h"

struct syscall syscall_timer_create = {
	.name = "timer_create",
	.num_args = 3,
	.arg1name = "which_clock",
	.arg2name = "timer_event_spec",
	.arg2type = ARG_ADDRESS,
	.arg3name = "create_timer_id",
	.arg3type = ARG_ADDRESS,
};
