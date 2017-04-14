#include <stdlib.h>
#include <net/if.h>
#include <linux/llc.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "config.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_LLC 268

#ifndef USE_LLC_OPT_PKTINFO
#define LLC_OPT_PKTINFO LLC_OPT_UNKNOWN
#endif

#define NR_SOL_LLC_OPTS ARRAY_SIZE(llc_opts)
static const unsigned int llc_opts[] = {
	LLC_OPT_RETRY, LLC_OPT_SIZE, LLC_OPT_ACK_TMR_EXP, LLC_OPT_P_TMR_EXP,
	LLC_OPT_REJ_TMR_EXP, LLC_OPT_BUSY_TMR_EXP, LLC_OPT_TX_WIN, LLC_OPT_RX_WIN,
	LLC_OPT_PKTINFO
};

void llc_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_LLC;

	val = rand() % NR_SOL_LLC_OPTS;
	so->optname = llc_opts[val];
}
