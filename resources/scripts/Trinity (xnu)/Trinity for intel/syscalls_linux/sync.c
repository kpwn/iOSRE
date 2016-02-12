/*
 * SYSCALL_DEFINE0(sync)
 */
#include "sanitise.h"

struct syscall syscall_sync = {
	.name = "sync",
	.num_args = 0,
	.group = GROUP_VFS,
};
