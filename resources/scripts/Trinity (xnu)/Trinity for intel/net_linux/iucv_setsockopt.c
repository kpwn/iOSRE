#include <stdlib.h>
#include "net.h"
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_IUCV 277

#define NR_SOL_IUCV_OPTS ARRAY_SIZE(iucv_opts)
static const unsigned int iucv_opts[] = {
	SO_IPRMDATA_MSG, SO_MSGLIMIT, SO_MSGSIZE };

void iucv_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_IUCV;

	val = rand() % NR_SOL_IUCV_OPTS;
	so->optname = iucv_opts[val];

	so->optval = sizeof(int);
}
