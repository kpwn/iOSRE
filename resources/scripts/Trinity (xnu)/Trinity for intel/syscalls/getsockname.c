/*
 * SYSCALL_DEFINE3(getsockname, int, fd, struct sockaddr __user *, usockaddr, int __user *, usockaddr_len)
 */
#include "sanitise.h"

struct syscall syscall_getsockname = {
	.name = "getsockname",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "usockaddr",
	.arg2type = ARG_SOCKADDR,
	.arg3name = "usockaddr_len",
	.arg3type = ARG_SOCKADDRLEN,
	.flags = NEED_ALARM,
};
