/*
 * SYSCALL_DEFINE4(epoll_wait, int, epfd, struct epoll_event __user *, events, int, maxevents, int, timeout)
 *
 * When  successful, returns the number of file descriptors ready for the requested I/O,
 * or zero if no file descriptor became ready during the requested timeout milliseconds.
 * When an error occurs, returns -1 and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_epoll_wait = {
	.name = "epoll_wait",
	.num_args = 4,
	.arg1name = "epfd",
	.arg1type = ARG_FD,
	.arg2name = "events",
	.arg2type = ARG_ADDRESS,
	.arg3name = "maxevents",
	.arg4name = "timeout",
	.rettype = RET_BORING,
	.flags = NEED_ALARM,
};
