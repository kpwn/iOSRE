/*
 * SYSCALL_DEFINE2(shutdown, int, fd, int, how)
 */
#include <sys/socket.h>
#include "sanitise.h"

struct syscall syscall_shutdown = {
	.name = "shutdown",
	.num_args = 2,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "how",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 3,
		.values = { SHUT_RD, SHUT_WR, SHUT_RDWR },
	},
	.flags = NEED_ALARM,
};
