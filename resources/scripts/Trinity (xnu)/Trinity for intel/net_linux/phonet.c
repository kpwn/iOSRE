#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/phonet.h>
#include <stdlib.h>
#include "net.h"
#include "random.h"

void phonet_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_pn *pn;

	pn = malloc(sizeof(struct sockaddr_pn));
	if (pn == NULL)
		return;

	pn->spn_family = PF_PHONET;
	pn->spn_obj = rand();
	pn->spn_dev = rand();
	pn->spn_resource = rand();
	*addr = (unsigned long) pn;
	*addrlen = sizeof(struct sockaddr_pn);
}

void phonet_rand_socket(struct socket_triplet *st)
{
	st->protocol = 0;
	if (rand_bool())
		st->type = SOCK_DGRAM;
	else
		st->type = SOCK_SEQPACKET;
}
