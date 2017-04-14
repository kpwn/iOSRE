#include <stdlib.h>
#include <netrom/netrom.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_NETROM_OPTS ARRAY_SIZE(netrom_opts)
static const unsigned int netrom_opts[] = {
	NETROM_T1, NETROM_T2, NETROM_N2, NETROM_T4, NETROM_IDLE };

void netrom_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_NETROM;

	val = rand() % NR_SOL_NETROM_OPTS;
	so->optname = netrom_opts[val];
}
