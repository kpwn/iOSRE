#include <stdlib.h>
#include <netinet/udp.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_UDP_OPTS ARRAY_SIZE(udp_opts)
static const unsigned int udp_opts[] = { UDP_CORK, UDP_ENCAP };

void udp_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_UDP;

	val = rand() % NR_SOL_UDP_OPTS;
	so->optname = udp_opts[val];

	switch (so->optname) {
	case UDP_CORK:
		break;
	case UDP_ENCAP:
		page_rand[0] = (rand() % 3) + 1;        // Encapsulation types.
		break;
	default:
		break;
	}
}
