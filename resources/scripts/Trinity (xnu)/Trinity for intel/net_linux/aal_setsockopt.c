#include "net.h"

void aal_setsockopt(struct sockopt *so)
{
	so->level = SOL_AAL;
}
