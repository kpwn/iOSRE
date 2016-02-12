/*
 * SYSCALL_DEFINE3(syslog, int, type, char __user *, buf, int, len)
 */
#include "sanitise.h"

struct syscall syscall_syslog = {
	.name = "syslog",
	.num_args = 3,
	.arg1name = "type",
	.arg2name = "buf",
	.arg2type = ARG_ADDRESS,
	.arg3name = "len",
	.arg3type = ARG_LEN,
};
