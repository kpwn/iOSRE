/*
 * SYSCALL_DEFINE3(sigprocmask, int, how, old_sigset_t __user *, set,
                 old_sigset_t __user *, oset)
 */
#include "sanitise.h"

struct syscall syscall_sigprocmask = {
	.name = "sigprocmask",
	.num_args = 3,
	.arg1name = "how",
	.arg2name = "set",
	.arg2type = ARG_ADDRESS,
	.arg3name = "oset",
};
