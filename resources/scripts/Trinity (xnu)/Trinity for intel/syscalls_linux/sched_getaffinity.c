/*
 * SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned int, len,
	 unsigned long __user *, user_mask_ptr)
 */
#include "sanitise.h"

struct syscall syscall_sched_getaffinity = {
	.name = "sched_getaffinity",
	.num_args = 3,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.arg3name = "user_mask_ptr",
	.arg3type = ARG_ADDRESS,
};
