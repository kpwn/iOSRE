/*
 * SYSCALL_DEFINE2(clock_getres, const clockid_t, which_clock, struct timespec __user *, tp)
 *
 * return 0 for success, or -1 for failure (in which case errno is set appropriately).
 */
#include "sanitise.h"

struct syscall syscall_clock_getres = {
	.name = "clock_getres",
	.num_args = 2,
	.arg1name = "which_clock",
	.arg2name = "tp",
	.arg2type = ARG_ADDRESS,
	.rettype = RET_ZERO_SUCCESS,
};
