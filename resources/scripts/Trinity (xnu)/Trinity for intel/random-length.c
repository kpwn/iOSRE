#include <stdlib.h>

#include "arch.h"	// page_size
#include "sanitise.h"
#include "random.h"

unsigned long get_len(void)
{
	int i = 0;

	i = rand32();

	/* short circuit if 0 */
	if (i == 0)
		return 0;

	switch (rand() % 6) {

	case 0:	i &= 0xff;
		break;
	case 1: i &= page_size;
		break;
	case 2:	i &= 0xffff;
		break;
	case 3:	i &= 0xffffff;
		break;
	case 4:	i &= 0xffffffff;
		break;
	default:
		// Pass through
		break;
	}

	/* again, short circuit if 0 */
	if (i == 0)
		return 0;

	/* we might get lucky if something is counting ints/longs etc. */
	if (rand() % 100 < 25) {
		int _div = 1 << ((rand() % 4) + 1);      /* 2,4,8 or 16 */
		i /= _div;
	}

	return i;
}
