#include <stdio.h>
#include <stdlib.h>
#include "arch.h"
#include "log.h"	// for BUG
#include "random.h"
#include "sanitise.h"
#include "trinity.h"	// page_size

unsigned int get_interesting_32bit_value(void)
{
	switch (rand() % 11) {

	/* common case, return small values*/
	case 0 ... 7:
		switch (rand() % 9) {
		case 0:	return 0x00000000;
		case 1:	return 0x00000001;
		case 2:	return rand() % 256;
		case 3:	return 0x00000fff;	// 4095
		case 4:	return 0x00001000;	// 4096
		case 5:	return 0x00001001;	// 4097
		case 6:	return 0x00008000;
		case 7:	return 0x0000fffe;
		case 8:	return 0x0000ffff;
		default:
			BUG("unreachable!\n");
			return 0;
		}
		break;

	/* less common case, go crazy */
	case 8 ... 10:
		switch (rand() % 15) {
		case 0:	return 0x00010000;
		case 1:	return 0x0fffffff;
		case 2:	return 0x40000000;
		case 3:	return 0x7fffffff;
		case 4:	return 0x80000000;
		case 5:	return 0x80000001;
		case 6:	return 0x8fffffff;
		case 7: return 0xc0000000;
		case 8:	return 0xf0000000;
		case 9:	return 0xff000000;
		case 10: return 0xffff0000;
		case 11: return 0xffffe000;
		case 12: return 0xffffff00 | (rand() % 256);
		case 13: return 0xffffffff;
		case 14: return 0xffffffff - page_size;
		default:
			BUG("unreachable!\n");
			return 0;
		}
		break;

	default:
		BUG("unreachable!\n");
		break;
	}

	BUG("unreachable!\n");
	return 0;
}

#if __WORDSIZE != 32
static unsigned long per_arch_interesting_addr(unsigned long low)
{
	int i = 0;

#if defined(__x86_64__)
	i = rand() % 4;

	switch (i) {
	case 0: return 0x00007fffffffffffUL;			// x86-64 canonical addr end.
	case 1: return 0x0000800000000000UL;			// First x86-64 non-canonical addr
	case 2: return 0xffff800000000000UL | (low << 4);		// x86-64 canonical addr range 2 begin
	case 3: return VDSO_ADDR | (low & 0x0fffff);
	default:
		BUG("unreachable!\n");
		break;
	}
#endif

	// FIXME: Add more arch specific addresses here.

	return i | low;
}
#endif	/* __WORDSIZE */

unsigned long get_interesting_value(void)
{
#if __WORDSIZE == 32
	return get_interesting_32bit_value();
#else
	unsigned long low = 0;

	if (rand_bool())
		low = get_interesting_32bit_value();

	switch (rand() % 13) {
	case 0: return 0;
	case 1: return low;
	case 2: return 0x0000000100000000UL | low;
	case 3: return 0x7fffffff00000000UL | low;
	case 4: return 0x8000000000000000UL | low;
	case 5: return 0xffffffff00000000UL | low;
	case 6: return 0xffffffffffffff00UL | (rand() % 256);
	case 7: return 0xffffffffffffffffUL - page_size;
	case 8: return PAGE_OFFSET | (low << 4);
	case 9: return KERNEL_ADDR | (low & 0xffffff);
	case 10: return MODULE_ADDR | (low & 0xffffff);
	case 11: return per_arch_interesting_addr(low);
	case 12: return (low << 32);
	default: break;
	}
	BUG("unreachable!\n");
	return 0;
#endif	/* __WORDSIZE */
}
