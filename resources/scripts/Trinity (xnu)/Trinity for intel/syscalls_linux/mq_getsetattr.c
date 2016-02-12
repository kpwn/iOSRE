/*
 * SYSCALL_DEFINE3(mq_getsetattr, mqd_t, mqdes,
	const struct mq_attr __user *, u_mqstat,
	struct mq_attr __user *, u_omqstat)
 */
#include "sanitise.h"

struct syscall syscall_mq_getsetattr = {
	.name = "mq_getsetattr",
	.num_args = 3,
	.arg1name = "mqdes",
	.arg1type = ARG_FD,
	.arg2name = "u_mqstat",
	.arg2type = ARG_ADDRESS,
	.arg3name = "u_omqstat",
	.arg3type = ARG_ADDRESS,
	.flags = NEED_ALARM,
};
