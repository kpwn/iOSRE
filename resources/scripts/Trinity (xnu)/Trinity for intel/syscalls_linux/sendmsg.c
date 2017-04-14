/*
 * SYSCALL_DEFINE3(sendmsg, int, fd, struct msghdr __user *, msg, unsigned, flags)
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "compat.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"

static void sanitise_sendmsg(int childno)
{
	struct msghdr *msg;

	// FIXME: Convert to use generic ARG_IOVEC
        msg = malloc(sizeof(struct msghdr));
	if (msg == NULL) {
		shm->a2[childno] = (unsigned long) get_address();
		return;
	}

	msg->msg_name = get_address();
	msg->msg_namelen = get_len();
	msg->msg_iov = get_address();
	msg->msg_iovlen = get_len();
	msg->msg_control = get_address();
	msg->msg_controllen = get_len();
	msg->msg_flags = rand32();

	shm->a2[childno] = (unsigned long) msg;
}

struct syscall syscall_sendmsg = {
	.name = "sendmsg",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "msg",
	.arg3name = "flags",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 20,
		.values = { MSG_OOB, MSG_PEEK, MSG_DONTROUTE, MSG_CTRUNC,
				MSG_TRUNC, MSG_DONTWAIT, MSG_EOR,
				MSG_WAITALL, MSG_FIN, MSG_SYN, MSG_CONFIRM,
				MSG_RST, MSG_ERRQUEUE, MSG_NOSIGNAL, MSG_MORE,
				MSG_WAITFORONE, MSG_CMSG_CLOEXEC, MSG_FASTOPEN, MSG_CMSG_COMPAT },
	},
	.sanitise = sanitise_sendmsg,
	.flags = NEED_ALARM,
};
