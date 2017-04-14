#include <stdlib.h>
#include <sys/socket.h> /* old irda.h is broken */

/* old irda.h does not include something which defines sa_family_t */
#include <netinet/in.h>

#include <linux/irda.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_IRDA_OPTS ARRAY_SIZE(irda_opts)
static const unsigned int irda_opts[] = {
	IRLMP_ENUMDEVICES, IRLMP_IAS_SET, IRLMP_IAS_QUERY, IRLMP_HINTS_SET,
	IRLMP_QOS_SET, IRLMP_QOS_GET, IRLMP_MAX_SDU_SIZE, IRLMP_IAS_GET,
	IRLMP_IAS_DEL, IRLMP_HINT_MASK_SET, IRLMP_WAITDEVICE };

void irda_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_IRDA;

	val = rand() % NR_SOL_IRDA_OPTS;
	so->optname = irda_opts[val];
}
