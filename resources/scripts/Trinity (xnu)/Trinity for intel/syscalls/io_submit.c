/*
 * SYSCALL_DEFINE3(io_submit, aio_context_t, ctx_id, long, nr,
	 struct iocb __user * __user *, iocbpp)
 */
#include "sanitise.h"

struct syscall syscall_io_submit = {
	.name = "io_submit",
	.num_args = 3,
	.arg1name = "ctx_id",
	.arg2name = "nr",
	.arg2type = ARG_LEN,
	.arg3name = "iocbpp",
	.arg3type = ARG_ADDRESS,
	.flags = NEED_ALARM,
};
