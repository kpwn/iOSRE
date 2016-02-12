/*
 * SYSCALL_DEFINE5(pwritev, unsigned long, fd, const struct iovec __user *, vec,
	 unsigned long, vlen, unsigned long, pos_l, unsigned long, pos_h)
 */
#include "sanitise.h"

struct syscall syscall_pwritev = {
	.name = "pwritev",
	.num_args = 5,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "vec",
	.arg2type = ARG_IOVEC,
	.arg3name = "vlen",
	.arg3type = ARG_IOVECLEN,
	.arg4name = "pos_l",
	.arg5name = "pos_h",
	.flags = NEED_ALARM,
};
