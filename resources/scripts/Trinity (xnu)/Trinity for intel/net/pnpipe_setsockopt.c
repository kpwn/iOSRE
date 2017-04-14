#include <stdlib.h>
#include "net.h"

#define SOL_PNPIPE 275

void pnpipe_setsockopt(struct sockopt *so)
{
	so->level = SOL_PNPIPE;
}
