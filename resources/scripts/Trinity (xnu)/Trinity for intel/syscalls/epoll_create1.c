/*
 * SYSCALL_DEFINE1(epoll_create1, int, flags)
 *
 * On success, returns a nonnegative file descriptor.
 * On error, -1 is returned, and errno is set to indicate the error.
 */

#define EPOLL_CLOEXEC 02000000

#include "sanitise.h"

struct syscall syscall_epoll_create1 = {
	.name = "epoll_create1",
	.num_args = 1,
	.arg1name = "flags",
	.arg1type = ARG_LIST,
	.arg1list = {
		.num = 1,
		.values = { EPOLL_CLOEXEC },
	},
	.rettype = RET_FD,
};
