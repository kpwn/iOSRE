/*
 * SYSCALL_DEFINE5(io_getevents, aio_context_t, ctx_id,
	long, min_nr,
	long, nr,
	struct io_event __user *, events,
	struct timespec __user *, timeout)
 */
#include "sanitise.h"

struct syscall syscall_io_getevents = {
	.name = "io_getevents",
	.num_args = 5,
	.arg1name = "ctx_id",
	.arg2name = "min_nr",
	.arg2type = ARG_LEN,
	.arg3name = "nr",
	.arg3type = ARG_LEN,
	.arg4name = "events",
	.arg4type = ARG_ADDRESS,
	.arg5name = "timeout",
	.arg5type = ARG_ADDRESS,
};
