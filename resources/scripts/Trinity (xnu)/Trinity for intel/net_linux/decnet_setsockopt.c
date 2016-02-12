#include <stdlib.h>
#include <linux/dn.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_DECNET_OPTS ARRAY_SIZE(decnet_opts)
static const unsigned int decnet_opts[] = {
	SO_CONDATA, SO_CONACCESS, SO_PROXYUSR, SO_LINKINFO,
	DSO_CONDATA, DSO_DISDATA, DSO_CONACCESS, DSO_ACCEPTMODE,
	DSO_CONACCEPT, DSO_CONREJECT, DSO_LINKINFO, DSO_STREAM,
	DSO_SEQPACKET, DSO_MAXWINDOW, DSO_NODELAY, DSO_CORK,
	DSO_SERVICES, DSO_INFO
};

void decnet_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_DECNET;

	val = rand() % NR_SOL_DECNET_OPTS;
	so->optname = decnet_opts[val];

	// TODO: set optlen correctly

}
