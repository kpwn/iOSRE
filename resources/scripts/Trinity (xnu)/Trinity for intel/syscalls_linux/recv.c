/*
   asmlinkage long sys_recv(int fd, void __user *ubuf, size_t size,
                            unsigned flags)

 */
#include <sys/socket.h>
#include "compat.h"
#include "sanitise.h"

struct syscall syscall_recv = {
	.name = "recv",
	.num_args = 4,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "ubuf",
	.arg2type = ARG_ADDRESS,
	.arg3name = "size",
	.arg3type = ARG_LEN,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 20,
		.values = { MSG_DONTWAIT, MSG_ERRQUEUE, MSG_ERRQUEUE,
			    MSG_ERRQUEUE, MSG_OOB, MSG_PEEK, MSG_TRUNC,
			    MSG_WAITALL, MSG_EOR, MSG_TRUNC, MSG_CTRUNC,
			    MSG_OOB, MSG_ERRQUEUE },
	},
	.flags = NEED_ALARM,
};
