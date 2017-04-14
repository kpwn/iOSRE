/*
 * SYSCALL_DEFINE1(eventfd, unsigned int, count)
 *
 * On success, eventfd() returns a new eventfd file descriptor.
 * On error, -1 is returned and errno is set to indicate the error.
 */
#include "sanitise.h"

struct syscall syscall_eventfd = {
	.name = "eventfd",
	.num_args = 1,
	.arg1name = "count",
	.arg1type = ARG_LEN,
	.rettype = RET_FD,
};
