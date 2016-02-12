#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/atm.h>
#include <stdlib.h>
#include "net.h"

void atmpvc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_atmpvc *atmpvc;

	atmpvc = malloc(sizeof(struct sockaddr_atmpvc));
	if (atmpvc == NULL)
		return;

	atmpvc->sap_family = PF_ATMPVC;
	atmpvc->sap_addr.itf = rand();
	atmpvc->sap_addr.vpi = rand();
	atmpvc->sap_addr.vci = rand();
	*addr = (unsigned long) atmpvc;
	*addrlen = sizeof(struct sockaddr_atmpvc);
}

void atmsvc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_atmsvc *atmsvc;
	unsigned int i;

	atmsvc = malloc(sizeof(struct sockaddr_atmsvc));
	if (atmsvc == NULL)
		return;

	atmsvc->sas_family = PF_ATMSVC;
	for (i = 0; i < ATM_ESA_LEN; i++)
		atmsvc->sas_addr.prv[i] = rand();
	for (i = 0; i < ATM_E164_LEN; i++)
		atmsvc->sas_addr.pub[i] = rand();
	atmsvc->sas_addr.lij_type = rand();
	atmsvc->sas_addr.lij_id = rand();
	*addr = (unsigned long) atmsvc;
	*addrlen = sizeof(struct sockaddr_atmsvc);
}
