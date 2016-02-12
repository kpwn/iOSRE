/*
 * SYSCALL_DEFINE3(io_cancel, aio_context_t, ctx_id, struct iocb __user *, iocb,
	 struct io_event __user *, result)
 */
#include "sanitise.h"

struct syscall syscall_io_cancel = {
	.name = "io_cancel",
	.num_args = 3,
	.arg1name = "ctx_id",
	.arg2name = "iocb",
	.arg2type = ARG_ADDRESS,
	.arg3name = "result",
	.arg3type = ARG_ADDRESS,
};
