/*
 * SYSCALL_DEFINE4(mq_open, const char __user *, u_name, int, oflag, mode_t, mode, struct mq_attr __user *, u_a>
 */
#include "sanitise.h"

struct syscall syscall_mq_open = {
	.name = "mq_open",
	.num_args = 4,
	.arg1name = "u_name",
	.arg1type = ARG_ADDRESS,
	.arg2name = "oflag",
	.arg3name = "mode",
	.arg3type = ARG_MODE_T,
	.arg4name = "u_attr",
	.arg4type = ARG_ADDRESS,
};
