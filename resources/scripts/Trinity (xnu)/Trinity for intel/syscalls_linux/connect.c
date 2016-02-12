/*
 * SYSCALL_DEFINE3(connect, int, fd, struct sockaddr __user *, uservaddr, int, addrlen
 *
 * If the connection or binding succeeds, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_connect = {
	.name = "connect",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "uservaddr",
	.arg2type = ARG_SOCKADDR,
	.arg3name = "addrlen",
	.arg3type = ARG_SOCKADDRLEN,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
};
