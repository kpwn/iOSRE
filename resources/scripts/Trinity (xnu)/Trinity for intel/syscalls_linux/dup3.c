/*
 * SYSCALL_DEFINE3(dup3, unsigned int, oldfd, unsigned int, newfd, int, flags)
 *
 * On success, returns the new descriptor.
 * On error, -1 is returned, and errno is set appropriately.
 */

#include <unistd.h>
#include <fcntl.h>
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_dup3 = {
	.name = "dup3",
	.num_args = 3,
	.arg1name = "oldfd",
	.arg1type = ARG_FD,
	.arg2name = "newfd",
	.arg2type = ARG_FD,
	.arg3name = "flags",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 1,
		.values = { O_CLOEXEC },
	},
	.rettype = RET_FD,
	.flags = NEED_ALARM,
};
