#include <stdlib.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "random.h"

void x25_setsockopt(struct sockopt *so)
{
	so->level = SOL_X25;

	page_rand[0] = rand_bool();
	so->optval = sizeof(int);
}
