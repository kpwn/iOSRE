#include <linux/icmp.h>
#include "net.h"

void raw_setsockopt(struct sockopt *so)
{
	so->level = SOL_RAW;
	so->optname = ICMP_FILTER; // that's all (for now?)
}
