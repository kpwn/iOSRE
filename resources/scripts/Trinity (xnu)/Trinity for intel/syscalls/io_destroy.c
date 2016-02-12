/*
 * SYSCALL_DEFINE1(io_destroy, aio_context_t, ctx)
 */
#include "sanitise.h"

struct syscall syscall_io_destroy = {
	.name = "io_destroy",
	.num_args = 1,
	.arg1name = "ctx",
};
