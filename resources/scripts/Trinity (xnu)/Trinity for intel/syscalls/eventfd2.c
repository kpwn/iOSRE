/*
 * SYSCALL_DEFINE2(eventfd2, unsigned int, count, int, flags)
 *
 * On success, eventfd() returns a new eventfd file descriptor.
 * On error, -1 is returned and errno is set to indicate the error.
 */

#define EFD_SEMAPHORE 1
#define EFD_CLOEXEC 02000000
#define EFD_NONBLOCK 04000

#include "sanitise.h"

struct syscall syscall_eventfd2 = {
	.name = "eventfd2",
	.num_args = 2,
	.arg1name = "count",
	.arg1type = ARG_LEN,
	.arg2name = "flags",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 3,
		.values = { EFD_CLOEXEC, EFD_NONBLOCK, EFD_SEMAPHORE },
	},
	.rettype = RET_FD,
};
