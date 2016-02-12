/*
 * SYSCALL_DEFINE1(close, unsigned int, fd)
 *
 * returns zero on success.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_close = {
	.name = "close",
	.num_args = 1,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.flags = AVOID_SYSCALL,
	.rettype = RET_ZERO_SUCCESS,
};
