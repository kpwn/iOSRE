#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/ax25.h> /* for ax25_address in rose.h */
#include <linux/rose.h>
#include <stdlib.h>
#include "maps.h"	// page_rand
#include "net.h"

void rose_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_rose *rose;

	rose = malloc(sizeof(struct sockaddr_rose));
	if (rose == NULL)
		return;

	rose->srose_family = PF_ROSE;
	rose->srose_addr.rose_addr[0] = rand();
	rose->srose_addr.rose_addr[1] = rand();
	rose->srose_addr.rose_addr[2] = rand();
	rose->srose_addr.rose_addr[3] = rand();
	rose->srose_addr.rose_addr[4] = rand();

	memcpy(rose->srose_call.ax25_call, page_rand, 7);

	rose->srose_ndigis = rand();

	memcpy(rose->srose_digi.ax25_call, page_rand + 7, 7);

	*addr = (unsigned long) rose;
	*addrlen = sizeof(struct sockaddr_rose);
}
