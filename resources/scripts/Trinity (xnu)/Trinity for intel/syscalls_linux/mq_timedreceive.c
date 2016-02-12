/*
 * SYSCALL_DEFINE5(mq_timedreceive, mqd_t, mqdes, char __user *, u_msg_ptr,
	size_t, msg_len, unsigned int __user *, u_msg_prio,
	const struct timespec __user *, u_abs_timeout)
 */
#include "sanitise.h"

struct syscall syscall_mq_timedreceive = {
	.name = "mq_timedreceive",
	.num_args = 5,
	.arg1name = "mqdes",
	.arg1type = ARG_FD,
	.arg2name = "u_msg_ptr",
	.arg2type = ARG_ADDRESS,
	.arg3name = "msg_len",
	.arg3type = ARG_LEN,
	.arg4name = "u_msg_prio",
	.arg4type = ARG_ADDRESS,
	.arg5name = "u_abs_timeout",
	.arg5type = ARG_ADDRESS,
	.flags = NEED_ALARM,
};
