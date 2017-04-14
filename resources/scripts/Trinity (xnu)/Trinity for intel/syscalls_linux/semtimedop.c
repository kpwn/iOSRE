/*
 * SYSCALL_DEFINE4(semtimedop, int, semid, struct sembuf __user *, tsops,
	 unsigned, nsops, const struct timespec __user *, timeout)
 */
#include "sanitise.h"

struct syscall syscall_semtimedop = {
	.name = "semtimedop",
	.num_args = 4,
	.arg1name = "semid",
	.arg2name = "tsops",
	.arg2type = ARG_ADDRESS,
	.arg3name = "nsops",
	.arg4name = "timeout",
	.arg4type = ARG_ADDRESS,
};
