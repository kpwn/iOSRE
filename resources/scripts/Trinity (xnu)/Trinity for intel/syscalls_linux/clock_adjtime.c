/*
 *SYSCALL_DEFINE2(clock_adjtime, const clockid_t, which_clock,
 *		struct timex __user *, utx)
 */

#include "sanitise.h"

struct syscall syscall_clock_adjtime = {
	.name = "clock_adjtime",
	.num_args = 2,
	.arg1name = "which_clock",
	.arg1type = ARG_RANGE,
	.low1range = 0,
	.hi1range = 16,
	.arg2name = "utx",
	.arg2type = ARG_ADDRESS,
};
