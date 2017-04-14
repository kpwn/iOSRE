/*
 * SYSCALL_DEFINE3(bind, int, fd, struct sockaddr __user *, umyaddr, int, addrlen)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_bind = {
	.name = "bind",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "umyaddr",
	.arg2type = ARG_SOCKADDR,
	.arg3name = "addrlen",
	.arg3type = ARG_SOCKADDRLEN,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
};
