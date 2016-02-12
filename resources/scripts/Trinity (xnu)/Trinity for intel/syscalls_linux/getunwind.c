/*
  sys_getunwind (void __user *buf, size_t buf_size)
 */
#include "sanitise.h"

struct syscall syscall_getunwind = {
	.name = "getunwind",
	.num_args = 2,
	.flags = AVOID_SYSCALL, // IA-64 only
	.arg1name = "buf",
	.arg1type = ARG_ADDRESS,
	.arg2name = "buf_size",
	.arg2type = ARG_LEN,
};
