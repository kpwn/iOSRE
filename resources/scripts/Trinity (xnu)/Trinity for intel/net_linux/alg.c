#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "config.h"
#include "net.h"
#include "compat.h"

#ifdef USE_IF_ALG
#include <linux/if_alg.h>

void alg_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_alg *alg;
	unsigned int i;

	alg = malloc(sizeof(struct sockaddr_alg));
	if (alg == NULL)
		return;

	alg->salg_family = PF_ALG;
	for (i = 0; i < 14; i++)
		alg->salg_type[i] = rand();
	alg->salg_feat = rand();
	alg->salg_mask = rand();
	for (i = 0; i < 64; i++)
		alg->salg_name[i] = rand();
	*addr = (unsigned long) alg;
	*addrlen = sizeof(struct sockaddr_alg);
}

#endif
