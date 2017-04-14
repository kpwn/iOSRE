/*
 * SYSCALL_DEFINE6(recvfrom, int, fd, void __user *, ubuf, size_t, size,
	unsigned, flags, struct sockaddr __user *, addr,
	int __user *, addr_len)
 */
#include <sys/types.h>
#include <sys/socket.h>
#include "compat.h"
#include "sanitise.h"

struct syscall syscall_recvfrom = {
	.name = "recvfrom",
	.num_args = 6,
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
		.values = { MSG_OOB, MSG_PEEK, MSG_DONTROUTE, MSG_CTRUNC,
			    MSG_PROBE, MSG_TRUNC, MSG_DONTWAIT, MSG_EOR,
			    MSG_WAITALL, MSG_FIN, MSG_SYN, MSG_CONFIRM,
			    MSG_RST, MSG_ERRQUEUE, MSG_NOSIGNAL, MSG_MORE,
			    MSG_WAITFORONE, MSG_FASTOPEN, MSG_CMSG_CLOEXEC, MSG_CMSG_COMPAT },
	},
	.arg5name = "addr",
	.arg5type = ARG_SOCKADDR,
	.arg6name = "addr_len",
	.arg6type = ARG_SOCKADDRLEN,
	.flags = NEED_ALARM,
};
