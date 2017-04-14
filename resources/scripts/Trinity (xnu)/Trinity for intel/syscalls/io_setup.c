/*
 * SYSCALL_DEFINE2(io_setup, unsigned, nr_events, aio_context_t __user *, ctxp)
 */
#include "sanitise.h"

struct syscall syscall_io_setup = {
	.name = "io_setup",
	.num_args = 2,
	.arg1name = "nr_events",
	.arg2name = "ctxp",
	.arg2type = ARG_ADDRESS,
};
