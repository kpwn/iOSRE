/*
 * sys_poll(struct pollfd __user *ufds, unsigned int nfds, int timeout);
 */
#include "sanitise.h"

struct syscall syscall_poll = {
	.name = "poll",
	.num_args = 3,
	.arg1name = "ufds",
	.arg1type = ARG_ADDRESS,
	.arg2name = "nfds",
	.arg2type = ARG_LEN,
	.arg3name = "timeout_msecs",
	.arg3type = ARG_RANGE,
	.low3range = 0,
	.hi3range = 1,
	.flags = NEED_ALARM,
};
