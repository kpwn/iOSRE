#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/tipc.h>
#include <stdlib.h>
#include "net.h"
#include "random.h"

void tipc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_tipc *tipc;

	tipc = malloc(sizeof(struct sockaddr_tipc));
	if (tipc == NULL)
		return;
	tipc->family = AF_TIPC;
	tipc->addrtype = rand();
	tipc->scope = rand();
	tipc->addr.id.ref = rand();
	tipc->addr.id.node = rand();
	tipc->addr.nameseq.type = rand();
	tipc->addr.nameseq.lower = rand();
	tipc->addr.nameseq.upper = rand();
	tipc->addr.name.name.type = rand();
	tipc->addr.name.name.instance = rand();
	tipc->addr.name.domain = rand();
	*addr = (unsigned long) tipc;
	*addrlen = sizeof(struct sockaddr_tipc);
}

void tipc_rand_socket(struct socket_triplet *st)
{
	st->protocol = 0;

	switch (rand() % 3) {
	case 0: st->type = SOCK_STREAM;
		break;
	case 1: st->type = SOCK_SEQPACKET;
		break;
	case 2: st->type = SOCK_DGRAM;
		break;
	default: break;
	}
}
