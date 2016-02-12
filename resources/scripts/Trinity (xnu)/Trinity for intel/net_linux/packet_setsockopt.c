#include <stdlib.h>
#include <linux/if_packet.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_PACKET_OPTS ARRAY_SIZE(packet_opts)
static const unsigned int packet_opts[] = {
	PACKET_ADD_MEMBERSHIP, PACKET_DROP_MEMBERSHIP, PACKET_RECV_OUTPUT, 4,   /* Value 4 is still used by obsolete turbo-packet. */
	PACKET_RX_RING, PACKET_STATISTICS, PACKET_COPY_THRESH, PACKET_AUXDATA,
	PACKET_ORIGDEV, PACKET_VERSION, PACKET_HDRLEN, PACKET_RESERVE,
	PACKET_TX_RING, PACKET_LOSS, PACKET_VNET_HDR, PACKET_TX_TIMESTAMP,
	PACKET_TIMESTAMP, PACKET_FANOUT };

void packet_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_PACKET;

	val = rand() % NR_SOL_PACKET_OPTS;
	so->optname = packet_opts[val];

	/* Adjust length according to operation set. */
	switch (so->optname) {
	case PACKET_VERSION:
		page_rand[0] = rand() % 3; /* tpacket versions 1/2/3 */
		break;

	case PACKET_TX_RING:
	case PACKET_RX_RING:
#ifdef TPACKET3_HDRLEN
		if (rand() % 3 == 0)
			so->optlen = sizeof(struct tpacket_req3);
		else
#endif
			so->optlen = sizeof(struct tpacket_req);
		break;
	default:
		break;
	}
}
