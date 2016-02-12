/*
 * SYSCALL_DEFINE4(reboot, int, magic1, int, magic2, unsigned int, cmd, void __user *, arg)
 */
#include "sanitise.h"

struct syscall syscall_reboot = {
	.name = "reboot",
	.num_args = 4,
	.flags = CAPABILITY_CHECK,
	.arg1name = "magic1",
	.arg2name = "magic2",
	.arg3name = "cmd",
	.arg4name = "arg",
	.arg4type = ARG_ADDRESS,
};
