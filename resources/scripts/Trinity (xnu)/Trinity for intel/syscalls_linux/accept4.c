/*
 * SYSCALL_DEFINE4(accept4, int, fd, struct sockaddr __user *, upeer_sockaddr,
	 int __user *, upeer_addrlen, int, flags)
 *
 * On success, these system calls return a nonnegative integer that is a descriptor for the accepted socket.
 * On error, -1 is returned, and errno is set appropriately.
 *
 */

#define SOCK_CLOEXEC 02000000
#define SOCK_NONBLOCK 04000

#include "sanitise.h"

struct syscall syscall_accept4 = {
	.name = "accept4",
	.num_args = 4,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "upeer_sockaddr",
	.arg2type = ARG_SOCKADDR,
	.arg3name = "upeer_addrlen",
	.arg3type = ARG_SOCKADDRLEN,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 2,
		.values = { SOCK_NONBLOCK, SOCK_CLOEXEC },
	},
	.rettype = RET_FD,
	.flags = NEED_ALARM,
};
