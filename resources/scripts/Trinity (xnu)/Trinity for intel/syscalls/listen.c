/*
 * SYSCALL_DEFINE2(listen, int, fd, int, backlog)
 */
#include "sanitise.h"

struct syscall syscall_listen = {
	.name = "listen",
	.num_args = 2,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "backlog",
	.flags = NEED_ALARM,
};
