#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "net.h"
#include "random.h"

in_addr_t random_ipv4_address(void)
{
	int addr = 0;
	int class = 0;

	switch (rand() % 9) {
	case 0:	addr = 0;		/* 0.0.0.0 */
		break;
	case 1:	addr = 0x0a000000;	/* 10.0.0.0/8 */
		class = 8;
		break;
	case 2:	addr = 0x7f000001;	/* 127.0.0.0/8 */
		class = 8;
		break;
	case 3:	addr = 0xa9fe0000;	/* 169.254.0.0/16 (link-local) */
		class = 16;
		break;
	case 4:	addr = 0xac100000;	/* 172.16.0.0/12 */
		class = 12;
		break;
	case 5:	addr = 0xc0586300;	/* 192.88.99.0/24 (6to4 anycast) */
		class = 24;
		break;
	case 6:	addr = 0xc0a80000;	/* 192.168.0.0/16 */
		class = 16;
		break;
	case 7:	addr = 0xe0000000;	/* 224.0.0.0/24 (multicast)*/
		class = 24;
		break;
	case 8:	addr = 0xffffffff;	/* 255.255.255.255 */
		break;
	default:
		break;
	}

	if (rand() % 100 < 50) {
		switch (class) {
		case 8:	addr |= rand() % 0xffffff;
			break;
		case 12: addr |= rand() % 0xfffff;
			break;
		case 16: addr |= rand() % 0xffff;
			break;
		case 24: addr |= rand() % 0xff;
			break;
		default: break;
		}
	}
	return htonl(addr);
}

void ipv4_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_in *ipv4;

	ipv4 = malloc(sizeof(struct sockaddr_in));
	if (ipv4 == NULL)
		return;

	ipv4->sin_family = PF_INET;
	ipv4->sin_addr.s_addr = random_ipv4_address();
	ipv4->sin_port = rand() % 65535;
	*addr = (unsigned long) ipv4;
	*addrlen = sizeof(struct sockaddr_in);
}

void inet_rand_socket(struct socket_triplet *st)
{
	switch (rand() % 3) {
	case 0: st->type = SOCK_STREAM;     // TCP
		if (rand_bool())
			st->protocol = 0;
		else
			st->protocol = IPPROTO_TCP;
		break;

	case 1: st->type = SOCK_DGRAM;      // UDP
		if (rand_bool())
			st->protocol = 0;
		else
			st->protocol = IPPROTO_UDP;
		break;

	case 2: st->type = SOCK_RAW;
		st->protocol = rand() % PROTO_MAX;
		break;

	default:break;
	}
}
