#include <stdlib.h>
#include <linux/filter.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_SOCKET_OPTS ARRAY_SIZE(socket_opts)
static const unsigned int socket_opts[] = { SO_DEBUG, SO_REUSEADDR, SO_TYPE, SO_ERROR,
	SO_DONTROUTE, SO_BROADCAST, SO_SNDBUF, SO_RCVBUF,
	SO_SNDBUFFORCE, SO_RCVBUFFORCE, SO_KEEPALIVE, SO_OOBINLINE,
	SO_NO_CHECK, SO_PRIORITY, SO_LINGER, SO_BSDCOMPAT,
	SO_REUSEPORT, SO_PASSCRED, SO_PEERCRED, SO_RCVLOWAT, SO_SNDLOWAT,
	SO_RCVTIMEO, SO_SNDTIMEO, SO_SECURITY_AUTHENTICATION, SO_SECURITY_ENCRYPTION_TRANSPORT,
	SO_SECURITY_ENCRYPTION_NETWORK, SO_BINDTODEVICE, SO_ATTACH_FILTER, SO_DETACH_FILTER,
	SO_PEERNAME, SO_TIMESTAMP, SO_ACCEPTCONN, SO_PEERSEC,
	SO_PASSSEC, SO_TIMESTAMPNS, SO_MARK, SO_TIMESTAMPING,
	SO_PROTOCOL, SO_DOMAIN, SO_RXQ_OVFL, SO_WIFI_STATUS,
	SO_PEEK_OFF, SO_NOFCS, SO_LOCK_FILTER, SO_SELECT_ERR_QUEUE,
	SO_BUSY_POLL, SO_MAX_PACING_RATE };

void socket_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_SOCKET;

	val = rand() % NR_SOL_SOCKET_OPTS;
	so->optname = socket_opts[val];

	/* Adjust length according to operation set. */
	switch (so->optname) {

	case SO_LINGER:
		so->optlen = sizeof(struct linger);
		break;

	case SO_RCVTIMEO:
	case SO_SNDTIMEO:
		so->optlen = sizeof(struct timeval);
		break;

	case SO_ATTACH_FILTER:
		gen_bpf((unsigned long *) page_rand, NULL);
		so->optlen = sizeof(struct sock_fprog);
		break;

	default:
		break;
	}
}
