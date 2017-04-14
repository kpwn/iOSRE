#include <stdlib.h>
#include <sys/socket.h> /* old netlink.h is broken */

/* For sa_family_t needed by <linux/netlink.h> */
#include <netinet/in.h>

#include <linux/netlink.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_NETLINK 270

#define NR_SOL_NETLINK_OPTS ARRAY_SIZE(netlink_opts)
static const unsigned int netlink_opts[] = {
	NETLINK_ADD_MEMBERSHIP, NETLINK_DROP_MEMBERSHIP, NETLINK_PKTINFO, NETLINK_BROADCAST_ERROR,
	NETLINK_NO_ENOBUFS, NETLINK_RX_RING, NETLINK_TX_RING };

void netlink_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_NETLINK;

	val = rand() % NR_SOL_NETLINK_OPTS;
	so->optname = netlink_opts[val];
}
