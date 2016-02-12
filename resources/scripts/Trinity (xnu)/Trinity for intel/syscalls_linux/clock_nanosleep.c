/*
 * SYSCALL_DEFINE4(clock_nanosleep, const clockid_t, which_clock, int, flags,
	const struct timespec __user *, rqtp,
	struct timespec __user *, rmtp)
 *
 * On successfully sleeping for the requested interval, clock_nanosleep() returns 0.
 * If the call is interrupted by a signal handler or  encounters  an  error,
 *  then it returns one of the positive error number listed in ERRORS.
 */

#include <time.h>
#include "sanitise.h"

struct syscall syscall_clock_nanosleep = {
	.name = "clock_nanosleep",
	.num_args = 4,
	.arg1name = "which_clock",
	.arg1type = ARG_OP,
	.arg1list = {
		.num = 3,
		.values = { CLOCK_REALTIME, CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID },
	},
	.arg2name = "flags",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 1,
		.values = { TIMER_ABSTIME },
	},
	.arg3name = "rqtp",
	.arg3type = ARG_ADDRESS,
	.arg4name = "rmtp",
	.arg4type = ARG_ADDRESS,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
};
