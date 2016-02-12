/*
 * SYSCALL_DEFINE4(migrate_pages, pid_t, pid, unsigned long, maxnode,
	 const unsigned long __user *, old_nodes,
	 const unsigned long __user *, new_nodes)
 */
#include "sanitise.h"

struct syscall syscall_migrate_pages = {
	.name = "migrate_pages",
	.num_args = 4,
	.arg1name = "pid",
	.arg1type = ARG_PID,
	.arg2name = "maxnode",
	.arg3name = "old_nodes",
	.arg3type = ARG_ADDRESS,
	.arg4name = "new_nodes",
	.arg4type = ARG_ADDRESS,
	.group = GROUP_VM,
};
