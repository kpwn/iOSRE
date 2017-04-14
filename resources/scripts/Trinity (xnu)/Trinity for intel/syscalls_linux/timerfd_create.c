/*
 * SYSCALL_DEFINE2(timerfd_create, int, clockid, int, flags)
 */
#include <time.h>
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_timerfd_create = {
	.name = "timerfd_create",
	.num_args = 2,
	.arg1name = "clockid",
	.arg1type = ARG_OP,
	.arg1list = {
		.num = 2,
		.values = { CLOCK_REALTIME, CLOCK_MONOTONIC },
	},
	.arg2name = "flags",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 2,
		.values = { TFD_NONBLOCK, TFD_CLOEXEC },
	},
};
