/*
 * SYSCALL_DEFINE4(signalfd4, int, ufd, sigset_t __user *, user_mask,
	 size_t, sizemask, int, flags)
 */
#include "sanitise.h"

#define SFD_CLOEXEC 02000000
#define SFD_NONBLOCK 04000

struct syscall syscall_signalfd4 = {
	.name = "signalfd4",
	.num_args = 4,
	.arg1name = "ufd",
	.arg1type = ARG_FD,
	.arg2name = "user_mask",
	.arg2type = ARG_ADDRESS,
	.arg3name = "sizemask",
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 2,
		.values = { SFD_CLOEXEC , SFD_NONBLOCK },
	},
	.flags = NEED_ALARM,
};
