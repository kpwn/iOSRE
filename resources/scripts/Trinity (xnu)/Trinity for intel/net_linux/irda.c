#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/irda.h>
#include <stdlib.h>
#include "net.h"
#include "random.h"

void irda_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_irda *irda;
	unsigned int i;

	irda = malloc(sizeof(struct sockaddr_irda));
	if (irda == NULL)
		return;

	irda->sir_family = PF_IRDA;
	irda->sir_lsap_sel = rand();
	irda->sir_addr = rand();
	for (i = 0; i < 25; i++)
		irda->sir_name[i] = rand();
	*addr = (unsigned long) irda;
	*addrlen = sizeof(struct sockaddr_irda);
}

void irda_rand_socket(struct socket_triplet *st)
{
	switch (rand() % 3) {

	case 0: st->type = SOCK_STREAM;
		st->protocol = rand() % PROTO_MAX;
		break;

	case 1: st->type = SOCK_SEQPACKET;
		st->protocol = rand() % PROTO_MAX;
		break;

	case 2: st->type = SOCK_DGRAM;
		if (rand_bool())
			st->protocol = IRDAPROTO_ULTRA;
		else
			st->protocol = IRDAPROTO_UNITDATA;
		break;

	default:break;
	}
}
