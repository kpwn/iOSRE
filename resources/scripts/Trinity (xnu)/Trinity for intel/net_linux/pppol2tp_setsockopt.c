#include <stdlib.h>
#include <netinet/in.h>
#include <linux/if_pppol2tp.h>
#include "net.h"
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_PPPOL2TP 273

#define NR_SOL_PPPOL2TP_OPTS ARRAY_SIZE(pppol2tp_opts)
static const unsigned int pppol2tp_opts[] = {
	PPPOL2TP_SO_DEBUG, PPPOL2TP_SO_RECVSEQ, PPPOL2TP_SO_SENDSEQ, PPPOL2TP_SO_LNSMODE,
	PPPOL2TP_SO_REORDERTO };

void pppol2tp_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_PPPOL2TP;

	val = rand() % NR_SOL_PPPOL2TP_OPTS;
	so->optname = pppol2tp_opts[val];

	so->optval = sizeof(int);
}
