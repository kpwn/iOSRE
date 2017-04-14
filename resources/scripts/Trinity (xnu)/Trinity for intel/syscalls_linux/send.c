/*
 *  SYSCALL_DEFINE4(send, int, fd, void __user *, buff, size_t, len,
                unsigned, flags)
 */
#include <sys/socket.h>
#include "sanitise.h"

struct syscall syscall_send = {
	.name = "send",
	.num_args = 4,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "buff",
	.arg2type = ARG_ADDRESS,
	.arg3name = "len",
	.arg3type = ARG_LEN,
	.arg4name = "flags",
        .arg4type = ARG_LIST,
        .arg4list = {
                .num = 7,
                .values = { MSG_CONFIRM, MSG_DONTROUTE, MSG_DONTWAIT,
			    MSG_EOR, MSG_MORE, MSG_NOSIGNAL, MSG_OOB },
	},
};
