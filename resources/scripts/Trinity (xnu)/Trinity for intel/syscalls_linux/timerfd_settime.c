/*
 * SYSCALL_DEFINE4(timerfd_settime, int, ufd, int, flags,
	 const struct itimerspec __user *, utmr,
	 struct itimerspec __user *, otmr)
 */
#include "sanitise.h"

#define TFD_CLOEXEC 02000000
#define TFD_NONBLOCK 04000

struct syscall syscall_timerfd_settime = {
	.name = "timerfd_settime",
	.num_args = 4,
	.arg1name = "ufd",
	.arg1type = ARG_FD,
	.arg2name = "flags",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 2,
		.values = { TFD_NONBLOCK, TFD_CLOEXEC },
	},
	.arg3name = "utmr",
	.arg3type = ARG_ADDRESS,
	.arg4name = "otmr",
	.arg4type = ARG_ADDRESS,
	.flags = NEED_ALARM,
};
