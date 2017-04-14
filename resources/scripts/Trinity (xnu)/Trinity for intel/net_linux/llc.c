#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <linux/llc.h>
#include <stdlib.h>
#include "net.h"
#include "random.h"

void llc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_llc *llc;
	unsigned int i;

	llc = malloc(sizeof(struct sockaddr_llc));
	if (llc == NULL)
		return;
	llc->sllc_family = AF_LLC;
	llc->sllc_arphrd = ARPHRD_ETHER;
	llc->sllc_test = rand();
	llc->sllc_xid = rand();
	llc->sllc_ua = rand();
	llc->sllc_sap = rand();
	for (i = 0; i < IFHWADDRLEN; i++)
		llc->sllc_mac[i] = rand();
	*addr = (unsigned long) llc;
	*addrlen = sizeof(struct sockaddr_llc);
}

void llc_rand_socket(struct socket_triplet *st)
{
	st->protocol = rand() % PROTO_MAX;
	if (rand_bool())
		st->type = SOCK_STREAM;
	else
		st->type = SOCK_DGRAM;
}
