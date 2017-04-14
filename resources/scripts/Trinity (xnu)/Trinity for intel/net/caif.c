#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "config.h"
#include "net.h"
#include "random.h"
#include "compat.h"

#ifdef USE_CAIF
#include <linux/caif/caif_socket.h>

void caif_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_caif *caif;
	unsigned int i;

	caif = malloc(sizeof(struct sockaddr_caif));
	if (caif == NULL)
		return;

	caif->family = PF_CAIF;
	caif->u.at.type = rand();
	for (i = 0; i < 16; i++)
		caif->u.util.service[i] = rand();
	caif->u.dgm.connection_id = rand();
	caif->u.dgm.nsapi = rand();
	caif->u.rfm.connection_id = rand();
	for (i = 0; i < 16; i++)
		caif->u.rfm.volume[i] = rand();
	caif->u.dbg.type = rand();
	caif->u.dbg.service = rand();
	*addr = (unsigned long) caif;
	*addrlen = sizeof(struct sockaddr_caif);
}

void caif_rand_socket(struct socket_triplet *st)
{
	st->protocol = rand() % _CAIFPROTO_MAX;
	if (rand_bool())
		st->type = SOCK_SEQPACKET;
	else
		st->type = SOCK_STREAM;
}

#endif
