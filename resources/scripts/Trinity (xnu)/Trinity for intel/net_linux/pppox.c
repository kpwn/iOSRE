#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/if_ether.h> /* for ETH_ALEN in if_pppox.h */
#include <linux/if_pppox.h>
#include <stdlib.h>
#include "config.h"
#include "net.h"
#include "sanitise.h"

//TODO: Split out each case into separate function.

void pppox_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_pppox *pppox;
	struct sockaddr_pppol2tp *pppol2tp;
	unsigned int proto;
	unsigned int i;

	proto = rand() % 3;

	switch (proto) {

	case PX_PROTO_OE:
		pppox = malloc(sizeof(struct sockaddr_pppox));
		if (pppox == NULL)
			return;

		pppox->sa_family = PF_PPPOX;
		pppox->sa_protocol = proto;

		pppox->sa_addr.pppoe.sid = rand();
		for (i = 0; i < ETH_ALEN; i++)
			pppox->sa_addr.pppoe.remote[i] = rand();
		for (i = 0; i < IFNAMSIZ; i++)
			pppox->sa_addr.pppoe.dev[i] = rand();

#ifdef USE_PPPOX_PPTP
		pppox->sa_addr.pptp.call_id = rand();
		pppox->sa_addr.pptp.sin_addr.s_addr = random_ipv4_address();
#endif

		*addr = (unsigned long) pppox;
		*addrlen = sizeof(struct sockaddr_pppox);
		break;

	case PX_PROTO_OL2TP:
		switch (rand() % 4) {

		case 0:	/* PPPoL2TP */
			pppol2tp = malloc(sizeof(struct sockaddr_pppol2tp));
			if (pppol2tp == NULL)
				return;

			pppol2tp->sa_family = PF_PPPOX;
			pppol2tp->sa_protocol = proto;
			pppol2tp->pppol2tp.pid = get_pid();
			pppol2tp->pppol2tp.fd = get_random_fd();
			pppol2tp->pppol2tp.addr.sin_addr.s_addr = random_ipv4_address();
			pppol2tp->pppol2tp.s_tunnel = rand();
			pppol2tp->pppol2tp.s_session = rand();
			pppol2tp->pppol2tp.d_tunnel = rand();
			pppol2tp->pppol2tp.d_session = rand();
			*addr = (unsigned long) pppol2tp;
			*addrlen = sizeof(struct sockaddr_pppol2tp);
			break;

		case 1:	/* PPPoL2TPin6*/
#ifdef USE_PPPOL2TPIN6
			{
			struct sockaddr_pppol2tpin6 *pppol2tpin6;

			pppol2tpin6 = malloc(sizeof(struct sockaddr_pppol2tpin6));
			if (pppol2tpin6 == NULL)
				return;

			pppol2tpin6->sa_family = PF_PPPOX;
			pppol2tpin6->sa_protocol = proto;
			pppol2tpin6->pppol2tp.pid = get_pid();
			pppol2tpin6->pppol2tp.fd = get_random_fd();
			pppol2tpin6->pppol2tp.s_tunnel = rand();
			pppol2tpin6->pppol2tp.s_session = rand();
			pppol2tpin6->pppol2tp.d_tunnel = rand();
			pppol2tpin6->pppol2tp.d_session = rand();
			pppol2tpin6->pppol2tp.addr.sin6_family = AF_INET6;
			pppol2tpin6->pppol2tp.addr.sin6_port = rand();
			pppol2tpin6->pppol2tp.addr.sin6_flowinfo = rand();
			pppol2tpin6->pppol2tp.addr.sin6_addr.s6_addr32[0] = 0;
			pppol2tpin6->pppol2tp.addr.sin6_addr.s6_addr32[1] = 0;
			pppol2tpin6->pppol2tp.addr.sin6_addr.s6_addr32[2] = 0;
			pppol2tpin6->pppol2tp.addr.sin6_addr.s6_addr32[3] = htonl(1);
			pppol2tpin6->pppol2tp.addr.sin6_scope_id = rand();
			*addr = (unsigned long) pppol2tpin6;
			*addrlen = sizeof(struct sockaddr_pppol2tpin6);
			}
#endif
			break;

		case 2:	/* PPPoL2TPv3*/
#ifdef USE_PPPOL2TPV3
			{
			struct sockaddr_pppol2tpv3 *pppol2tpv3;

			pppol2tpv3 = malloc(sizeof(struct sockaddr_pppol2tpv3));
			if (pppol2tpv3 == NULL)
				return;

			pppol2tpv3->sa_family = PF_PPPOX;
			pppol2tpv3->sa_protocol = proto;
			pppol2tpv3->pppol2tp.pid = get_pid();
			pppol2tpv3->pppol2tp.fd = get_random_fd();
			pppol2tpv3->pppol2tp.addr.sin_addr.s_addr = random_ipv4_address();
			pppol2tpv3->pppol2tp.s_tunnel = rand();
			pppol2tpv3->pppol2tp.s_session = rand();
			pppol2tpv3->pppol2tp.d_tunnel = rand();
			pppol2tpv3->pppol2tp.d_session = rand();
			*addr = (unsigned long) pppol2tpv3;
			*addrlen = sizeof(struct sockaddr_pppol2tpv3);
			}
#endif
			break;

		case 3:	/* PPPoL2TPv3in6 */
#ifdef USE_PPPOL2TPIN6
			{
			struct sockaddr_pppol2tpv3in6 *pppol2tpv3in6;

			pppol2tpv3in6 = malloc(sizeof(struct sockaddr_pppol2tpv3in6));
			if (pppol2tpv3in6 == NULL)
				return;

			pppol2tpv3in6->sa_family = PF_PPPOX;
			pppol2tpv3in6->sa_protocol = proto;
			pppol2tpv3in6->pppol2tp.pid = get_pid();
			pppol2tpv3in6->pppol2tp.fd = get_random_fd();
			pppol2tpv3in6->pppol2tp.s_tunnel = rand();
			pppol2tpv3in6->pppol2tp.s_session = rand();
			pppol2tpv3in6->pppol2tp.d_tunnel = rand();
			pppol2tpv3in6->pppol2tp.d_session = rand();
			pppol2tpv3in6->pppol2tp.addr.sin6_family = AF_INET6;
			pppol2tpv3in6->pppol2tp.addr.sin6_port = rand();
			pppol2tpv3in6->pppol2tp.addr.sin6_flowinfo = rand();
			pppol2tpv3in6->pppol2tp.addr.sin6_addr.s6_addr32[0] = 0;
			pppol2tpv3in6->pppol2tp.addr.sin6_addr.s6_addr32[1] = 0;
			pppol2tpv3in6->pppol2tp.addr.sin6_addr.s6_addr32[2] = 0;
			pppol2tpv3in6->pppol2tp.addr.sin6_addr.s6_addr32[3] = random_ipv4_address();
			pppol2tpv3in6->pppol2tp.addr.sin6_scope_id = rand();
			*addr = (unsigned long) pppol2tpv3in6;
			*addrlen = sizeof(struct sockaddr_pppol2tpv3in6);
			}
#endif
			break;

		default:
			break;
		}


#ifdef USE_PPPOX_PPTP
	case PX_PROTO_PPTP:
		//FIXME: What do we do here?
		break;
#endif

	default:
		break;
	}
}
