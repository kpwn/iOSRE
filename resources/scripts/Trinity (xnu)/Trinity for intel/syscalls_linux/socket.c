/*
 * SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol)
 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "compat.h"
#include "log.h"
#include "net.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"
#include "config.h"
#include "params.h"
#include "protocols.h"
#include "trinity.h"

struct socket_ptr {
	unsigned int family;
	void (*func)(struct socket_triplet *st);
};
static const struct socket_ptr socketptrs[] = {
	{ .family = AF_APPLETALK, .func = &atalk_rand_socket },
	{ .family = AF_AX25, .func = &ax25_rand_socket },
#ifdef USE_CAIF
	{ .family = AF_CAIF, .func = &caif_rand_socket },
#endif
	{ .family = AF_CAN, .func = &can_rand_socket },
	{ .family = AF_DECnet, .func = &decnet_rand_socket },
	{ .family = AF_INET, .func = &inet_rand_socket },
	{ .family = AF_INET6, .func = &inet6_rand_socket },
	{ .family = AF_IPX, .func = &ipx_rand_socket },
	{ .family = AF_IRDA, .func = &irda_rand_socket },
	{ .family = AF_LLC, .func = &llc_rand_socket },
	{ .family = AF_NETLINK, .func = &netlink_rand_socket },
	{ .family = AF_NFC, .func = &nfc_rand_socket },
//TODO	{ .family = AF_IB, .func = &ib_rand_socket },
	{ .family = AF_PACKET, .func = &packet_rand_socket },
	{ .family = AF_PHONET, .func = &phonet_rand_socket },
	{ .family = AF_RDS, .func = &rds_rand_socket },
	{ .family = AF_TIPC, .func = &tipc_rand_socket },
	{ .family = AF_UNIX, .func = &unix_rand_socket },
	{ .family = AF_X25, .func = &x25_rand_socket },
};

void rand_proto_type(struct socket_triplet *st)
{
	int n;

	/*
	 * One special moment on packet sockets. They
	 * can be created with SOCK_PACKET, so if
	 * PF_PACKET is disabled, choose some other type.
	 */

	st->protocol = rand() % PROTO_MAX;

	if (st->family == PF_INET && no_protos[PF_PACKET])
		n = 5;
	else
		n = 6;

	switch (rand() % n) {
	case 0:	st->type = SOCK_DGRAM;	break;
	case 1:	st->type = SOCK_STREAM;	break;
	case 2:	st->type = SOCK_SEQPACKET;	break;
	case 3:	st->type = SOCK_RAW;	break;
	case 4:	st->type = SOCK_RDM;	break;
	/*
	 * Make sure it's last one.
	 */
	case 5:	st->type = SOCK_PACKET;	break;
	default: break;
	}
}

/* note: also called from generate_sockets() */
int sanitise_socket_triplet(struct socket_triplet *st)
{
	unsigned int i;
	for (i = 0; i < ARRAY_SIZE(socketptrs); i++) {
		if (socketptrs[i].family == st->family) {
			socketptrs[i].func(st);
			return 0;
		}
	}

	/* Couldn't find func, fall back to random. */
	return -1;
}

/* note: also called from sanitise_socketcall() */
void gen_socket_args(struct socket_triplet *st)
{
	if (do_specific_proto == TRUE)
		st->family = specific_proto;

	else {
		st->family = rand() % TRINITY_PF_MAX;

		/*
		 * If we get a disabled family, try to find
		 * first next allowed.
		 */
		BUG_ON(st->family >= ARRAY_SIZE(no_protos));
		if (no_protos[st->family]) {
			st->family = find_next_enabled_proto(st->family);
			if (st->family == -1u) {
				outputerr("No available socket family found\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	/* sometimes, still gen rand crap */
	if ((rand() % 100) == 0) {
		rand_proto_type(st);
		goto done;
	}

	/* otherwise.. sanitise based on the family. */
	if (sanitise_socket_triplet(st) < 0)
		rand_proto_type(st);	/* Couldn't find func, fall back to random. */


done:
	if ((rand() % 100) < 25)
		st->type |= SOCK_CLOEXEC;
	if ((rand() % 100) < 25)
		st->type |= SOCK_NONBLOCK;
}


static void sanitise_socket(int childno)
{
	struct socket_triplet st = { .family = 0, .type = 0, .protocol = 0 };

	gen_socket_args(&st);

	shm->a1[childno] = st.family;
	shm->a2[childno] = st.type;
	shm->a3[childno] = st.protocol;
}

struct syscall syscall_socket = {
	.name = "socket",
	.num_args = 3,
	.arg1name = "family",
	.arg2name = "type",
	.arg3name = "protocol",
	.sanitise = sanitise_socket,
};
