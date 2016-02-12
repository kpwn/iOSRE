/*
 * SYSCALL_DEFINE3(nfsservctl, int, cmd, struct nfsctl_arg __user *, arg, void __user *, res
 */
#include "sanitise.h"

struct syscall syscall_nfsservctl = {
	.name = "nfsservctl",
	.num_args = 3,
	.arg1name = "cmd",
	.arg2name = "arg",
	.arg2type = ARG_ADDRESS,
	.arg3name = "res",
	.arg3type = ARG_ADDRESS,
};
