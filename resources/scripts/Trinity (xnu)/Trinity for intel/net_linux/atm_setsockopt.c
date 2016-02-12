#include <stdlib.h>
#include <linux/atmdev.h>
#include <linux/atm.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_ATM_OPTS ARRAY_SIZE(atm_opts)
static const unsigned int atm_opts[] = {
	SO_SETCLP, SO_CIRANGE, SO_ATMQOS, SO_ATMSAP, SO_ATMPVC, SO_MULTIPOINT };

void atm_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_ATM;

	val = rand() % NR_SOL_ATM_OPTS;
	so->optname = atm_opts[val];
}
