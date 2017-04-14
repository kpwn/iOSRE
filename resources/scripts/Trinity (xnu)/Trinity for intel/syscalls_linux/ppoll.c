/*
 * SYSCALL_DEFINE5(ppoll, struct pollfd __user *, ufds, unsigned int, nfds,
	 struct timespec __user *, tsp, const sigset_t __user *, sigmask, size_t, sigsetsize) 
 */
#include "sanitise.h"

struct syscall syscall_ppoll = {
	.name = "ppoll",
	.num_args = 5,
	.arg1name = "ufds",
	.arg1type = ARG_ADDRESS,
	.arg2name = "nfds",
	.arg2type = ARG_LEN,
	.arg3name= "tsp",
	.arg3type = ARG_ADDRESS,
	.arg4name = "sigmask",
	.arg4type = ARG_ADDRESS,
	.arg5name = "sigsetsize",
	.flags = NEED_ALARM,
};
