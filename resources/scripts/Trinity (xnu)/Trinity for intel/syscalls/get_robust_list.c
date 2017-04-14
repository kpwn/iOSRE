/*
 * SYSCALL_DEFINE3(get_robust_list, int, pid,
	struct robust_list_head __user * __user *, head_ptr,
	size_t __user *, len_ptr)
 */
#include "sanitise.h"

struct syscall syscall_get_robust_list = {
	.name = "get_robust_list",
	.num_args = 3,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "head_ptr",
	.arg2type = ARG_ADDRESS,
	.arg3name = "len_ptr",
	.arg3type = ARG_ADDRESS,
};
