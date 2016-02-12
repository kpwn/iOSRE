#include "net.h"

#define SOL_ALG 279

void alg_setsockopt(struct sockopt *so)
{
	so->level = SOL_ALG;
}
