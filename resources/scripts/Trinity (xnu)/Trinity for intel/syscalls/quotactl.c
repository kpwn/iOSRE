/*
 * SYSCALL_DEFINE4(quotactl, unsigned int, cmd, const char __user *, special,
	 qid_t, id, void __user *, addr)
 */
#include "sanitise.h"

struct syscall syscall_quotactl = {
	.name = "quotactl",
	.num_args = 4,
	.arg1name = "cmd",
	.arg2name = "special",
	.arg2type = ARG_ADDRESS,
	.arg3name = "id",
	.arg4name = "addr",
	.arg4type = ARG_ADDRESS,
	.group = GROUP_VFS,
};
