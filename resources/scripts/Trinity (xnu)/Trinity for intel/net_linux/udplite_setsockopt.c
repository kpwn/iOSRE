#include <stdlib.h>
#include <linux/udp.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_UDPLITE 136

#define NR_SOL_UDPLITE_OPTS ARRAY_SIZE(udplite_opts)
static const unsigned int udplite_opts[] = { UDP_CORK, UDP_ENCAP, UDPLITE_SEND_CSCOV, UDPLITE_RECV_CSCOV };

void udplite_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_UDPLITE;

	val = rand() % NR_SOL_UDPLITE_OPTS;
	so->optname = udplite_opts[val];

	switch (so->optname) {
	case UDP_CORK:
		break;
	case UDP_ENCAP:
		page_rand[0] = (rand() % 3) + 1;        // Encapsulation types.
		break;
	case UDPLITE_SEND_CSCOV:
		break;
	case UDPLITE_RECV_CSCOV:
		break;
	default:
		break;
	}
}
