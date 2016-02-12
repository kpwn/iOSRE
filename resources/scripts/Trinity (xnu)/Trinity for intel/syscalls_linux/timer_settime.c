/*
 * SYSCALL_DEFINE4(timer_settime, timer_t, timer_id, int, flags,
	const struct itimerspec __user *, new_setting,
	struct itimerspec __user *, old_setting)
 */
#include <time.h>
#include "sanitise.h"

struct syscall syscall_timer_settime = {
	.name = "timer_settime",
	.num_args = 4,
	.arg1name = "timer_id",
	.arg2name = "flags",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 1,
		.values = { TIMER_ABSTIME },
	},
	.arg3name = "new_setting",
	.arg3type = ARG_ADDRESS,
	.arg4name = "old_setting",
	.arg4type = ARG_ADDRESS,
};
