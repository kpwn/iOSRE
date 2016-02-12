/*
 * SYSCALL_DEFINE1(dup, unsigned int, fildes)
 *
 * On success, returns the new descriptor.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_dup = {
	.name = "dup",
	.num_args = 1,
	.arg1name = "fildes",
	.arg1type = ARG_FD,
	.rettype = RET_FD,
	.flags = NEED_ALARM,
};
