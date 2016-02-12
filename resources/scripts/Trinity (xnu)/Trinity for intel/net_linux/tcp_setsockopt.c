#include <stdlib.h>
#include <linux/tcp.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_TCP 6

#define NR_SOL_TCP_OPTS ARRAY_SIZE(tcp_opts)
static const unsigned int tcp_opts[] = { TCP_NODELAY, TCP_MAXSEG, TCP_CORK, TCP_KEEPIDLE,
	TCP_KEEPINTVL, TCP_KEEPCNT, TCP_SYNCNT, TCP_LINGER2,
	TCP_DEFER_ACCEPT, TCP_WINDOW_CLAMP, TCP_INFO, TCP_QUICKACK,
	TCP_CONGESTION, TCP_MD5SIG, TCP_COOKIE_TRANSACTIONS, TCP_THIN_LINEAR_TIMEOUTS,
	TCP_THIN_DUPACK, TCP_USER_TIMEOUT, TCP_REPAIR, TCP_REPAIR_QUEUE,
	TCP_QUEUE_SEQ, TCP_REPAIR_OPTIONS, TCP_FASTOPEN};

void tcp_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_TCP;

	val = rand() % NR_SOL_TCP_OPTS;
	so->optname = tcp_opts[val];
}
