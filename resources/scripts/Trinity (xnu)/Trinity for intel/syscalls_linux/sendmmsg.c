/*
 * SYSCALL_DEFINE4(sendmmsg, int, fd, struct mmsghdr __user *, mmsg,
 *	unsigned int, vlen, unsigned int, flags)
 */
#include <sys/socket.h>
#include "compat.h"
#include "sanitise.h"

struct syscall syscall_sendmmsg = {
	.name = "sendmmsg",
	.num_args = 4,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "mmsg",
	.arg2type = ARG_ADDRESS,
	.arg3name = "vlen",
	.arg3type = ARG_LEN,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 20,
		.values = { MSG_OOB, MSG_PEEK, MSG_DONTROUTE, MSG_CTRUNC,
				MSG_PROBE, MSG_TRUNC, MSG_DONTWAIT, MSG_EOR,
				MSG_WAITALL, MSG_FIN, MSG_SYN, MSG_CONFIRM,
				MSG_RST, MSG_ERRQUEUE, MSG_NOSIGNAL, MSG_MORE,
				MSG_WAITFORONE, MSG_CMSG_CLOEXEC, MSG_FASTOPEN, MSG_CMSG_COMPAT },
	},
	.flags = NEED_ALARM,
};
