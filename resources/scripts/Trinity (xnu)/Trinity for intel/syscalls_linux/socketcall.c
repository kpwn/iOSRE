/*
 * SYSCALL_DEFINE2(socketcall, int, call, unsigned long __user *, args)
 */
#include <stdlib.h>
#include <linux/net.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "compat.h"
#include "net.h"
#include "sanitise.h"
#include "shm.h"
#include "trinity.h"

static void socketcall_socket(unsigned long *args)
{
	struct socket_triplet st;

	gen_socket_args(&st);

	args[0] = st.family;
	args[1] = st.type;
	args[2] = st.protocol;
}

struct socketcall_ptr {
        unsigned int call;
        void (*func)(unsigned long *args);
};

static const struct socketcall_ptr socketcallptrs[] = {
	{ .call = SYS_SOCKET, .func = socketcall_socket },
//	{ .call = SYS_BIND, .func = socketcall_bind },
//	{ .call = SYS_CONNECT, .func = socketcall_connect },
//	{ .call = SYS_LISTEN, .func = socketcall_listen },
//	{ .call = SYS_ACCEPT, .func = socketcall_accept },
//	{ .call = SYS_GETSOCKNAME, .func = socketcall_getsockname },
//	{ .call = SYS_GETPEERNAME, .func = socketcall_getpeername },
//	{ .call = SYS_SOCKETPAIR, .func = socketcall_socketpair },
//	{ .call = SYS_SEND, .func = socketcall_send },
//	{ .call = SYS_RECV, .func = socketcall_recv },
//	{ .call = SYS_SENDTO, .func = socketcall_sendto },
//	{ .call = SYS_RECVFROM, .func = socketcall_recvfrom },
//	{ .call = SYS_SHUTDOWN, .func = socketcall_shutdown },
//	{ .call = SYS_SETSOCKOPT, .func = socketcall_setsockopt },
//	{ .call = SYS_GETSOCKOPT, .func = socketcall_getsockopt },
//	{ .call = SYS_SENDMSG, .func = socketcall_sendmsg },
//	{ .call = SYS_RECVMSG, .func = socketcall_recvmsg },
//	{ .call = SYS_ACCEPT4, .func = socketcall_accept },
//	{ .call = SYS_RECVMMSG, .func = socketcall_recvmmsg },
//	{ .call = SYS_SENDMMSG, .func = socketcall_sendmmsg },
};


static void sanitise_socketcall(int childno)
{
	unsigned long *args;
	unsigned int i;

	args = malloc(6 * sizeof(unsigned long));

	shm->a1[childno] = rand() % 20;
	shm->a1[childno] = SYS_SOCKET;

	for (i = 0; i < ARRAY_SIZE(socketcallptrs); i++) {
		if (socketcallptrs[i].call == shm->a1[childno])
			socketcallptrs[i].func(args);
	}

	shm->a2[childno] = (unsigned long) args;
}

struct syscall syscall_socketcall = {
	.name = "socketcall",
	.num_args = 2,
	.arg1name = "call",
	.arg2name = "args",
	.arg2type = ARG_ADDRESS,
	.sanitise = sanitise_socketcall,
};
