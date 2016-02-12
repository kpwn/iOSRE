#include <stdlib.h>
#include <netax25/ax25.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_AX25_OPTS ARRAY_SIZE(ax25_opts)
static const unsigned int ax25_opts[] = {
	AX25_WINDOW, AX25_T1, AX25_N2, AX25_T3,
	AX25_T2, AX25_BACKOFF, AX25_EXTSEQ, AX25_PIDINCL,
	AX25_IDLE, AX25_PACLEN, AX25_IAMDIGI,
	SO_BINDTODEVICE };

void ax25_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_AX25;

	val = rand() % NR_SOL_AX25_OPTS;
	so->optname = ax25_opts[val];
}
