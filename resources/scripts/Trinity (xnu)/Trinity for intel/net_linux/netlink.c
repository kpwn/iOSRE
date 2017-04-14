#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include "compat.h"
#include "net.h"
#include "random.h"
#include "sanitise.h"

/* Current highest netlink socket. Supports some older kernels. */
#ifdef NETLINK_CRYPTO
#define _NETLINK_MAX NETLINK_CRYPTO
#else
	#ifdef NETLINK_RDMA
	#define _NETLINK_MAX NETLINK_RDMA
	#else
		#define _NETLINK_MAX NETLINK_ECRYPTFS
	#endif /* NETLINK_RDMA */
#endif /* NETLINK_CRYPTO */

void netlink_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_nl *nl;

	nl = malloc(sizeof(struct sockaddr_nl));
	if (nl == NULL)
		return;

	nl->nl_family = PF_NETLINK;
	nl->nl_pid = get_pid();
	nl->nl_groups = rand32();
	*addr = (unsigned long) nl;
	*addrlen = sizeof(struct sockaddr_nl);
}

void netlink_rand_socket(struct socket_triplet *st)
{
	if (rand_bool())
		st->type = SOCK_RAW;
	else
		st->type = SOCK_DGRAM;

	st->protocol = rand() % (_NETLINK_MAX + 1);
}
