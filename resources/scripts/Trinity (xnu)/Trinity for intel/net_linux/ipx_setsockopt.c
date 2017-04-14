#include <netipx/ipx.h>
#include "net.h"

void ipx_setsockopt(struct sockopt *so)
{
	so->level = SOL_IPX;
	so->optname = IPX_TYPE;
}
