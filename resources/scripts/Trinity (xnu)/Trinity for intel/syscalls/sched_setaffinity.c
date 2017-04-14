/*
 * SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned int, len,
	 unsigned long __user *, user_mask_ptr)
 */
#include "sanitise.h"

struct syscall syscall_sched_setaffinity = {
	.name = "sched_setaffinity",
	.num_args = 3,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.arg3name = "user_mask_ptr",
	.arg3type = ARG_ADDRESS,
};
