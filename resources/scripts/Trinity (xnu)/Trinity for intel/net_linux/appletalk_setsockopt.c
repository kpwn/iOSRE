#include <netatalk/at.h>
#include "net.h"

void atalk_setsockopt(struct sockopt *so)
{
	so->level = SOL_ATALK;
}
