/*
 * SYSCALL_DEFINE2(setns, int, fd, int, nstype)
 */
#include <sched.h>
#include "sanitise.h"

struct syscall syscall_setns= {
	.name = "setns",
	.num_args = 2,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "nstype",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 4,
		.values = { 0, CLONE_NEWIPC, CLONE_NEWNET, CLONE_NEWUTS, },
	},
	.flags = NEED_ALARM,
};
