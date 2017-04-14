/*
 * Routines to get randomness.
 */
#include <syslog.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include "pids.h"
#include "random.h"
#include "sanitise.h"	// interesting_numbers
#include "types.h"

unsigned int rand_bool(void)
{
	return rand() % 2;
}

static unsigned int rand_single_bit(unsigned char size)
{
	return (1L << (rand() % size));
}

/*
 * set N bits, where N= rand(0 - WORDSIZE/2)
 */
static unsigned long randbits(int limit)
{
	unsigned int num = rand() % limit / 2;
	unsigned int i;
	unsigned long r = 0;

	for (i = 0; i < num; i++)
		r |= (1 << (rand() % (limit - 1)));

	return r;
}

/*
 * Based on very similar routine stolen from iknowthis. Thanks Tavis.
 */
static unsigned long taviso(void)
{
	unsigned long r = 0;

	switch (rand() % 4) {
	case 0:	r = rand() & rand();
#if __WORDSIZE == 64
		r <<= 32;
		r |= rand() & rand();
#endif
		break;

	case 1:	r = rand() % rand();
#if __WORDSIZE == 64
		r <<= 32;
		r |= rand() % rand();
#endif
		break;

	case 2:	r = rand() | rand();
#if __WORDSIZE == 64
		r <<= 32;
		r |= rand() | rand();
#endif
		break;

	case 3:	r = rand();
#if __WORDSIZE == 64
		r <<= 32;
		r |= rand();
#endif
		break;

	default:
		break;
	}

	return r;
}

/*
 * Pick 8 random bytes, and concatenate them into a long.
 */
static unsigned long rand8x8(void)
{
	unsigned long r = 0UL;
	unsigned int i;

	for (i = (rand() % 7) + 1; i > 0; --i)
		r = (r << 8) | rand() % 256;

	return r;
}

/*
 * Pick 1 random byte, and repeat it through a long.
 */
static unsigned long rept8(unsigned int num)
{
	unsigned long r = 0UL;
	unsigned int i;
	unsigned char c;

	c = rand() % 256;
	for (i = rand() % (num - 1) ; i > 0; --i)
		r = (r << 8) | c;

	return r;
}

/*
 * "selector" function for 32bit random.
 * only called from rand32()
 */
static unsigned int __rand32(void)
{
	unsigned long r = 0;

	switch (rand() % 7) {
	case 0: r = rand_single_bit(32);
		break;
	case 1:	r = randbits(32);
		break;
	case 2: r = rand();
		break;
	case 3:	r = taviso();
		break;
	case 4:	r = rand8x8();
		break;
	case 5:	r = rept8(4);
		break;
	case 6:	return get_interesting_32bit_value();
	default:
		break;
	}

	return r;
}

/*
 * Generate, and munge a 32bit number.
 */
unsigned int rand32(void)
{
	unsigned long r = 0;

	r = __rand32();

	if (rand_bool()) {
		unsigned int i;
		unsigned int rounds;

		/* mangle it. */
		rounds = rand() % 3;
		for (i = 0; i < rounds; i++) {
			switch (rand_bool()) {
			case 0: r |= __rand32();
				break;
			case 1: r ^= __rand32();
				break;
			default:
				break;
			}
		}
	}

	/* Sometimes deduct it from INT_MAX */
	if (rand_bool())
		r = INT_MAX - r;

	/* Sometimes flip sign */
	if (rand_bool())
		r |= (1L << 31);

	/* we might get lucky if something is counting ints/longs etc. */
	if (rand() % 100 < 25) {
		int _div = 1 << ((rand() % 4) + 1);	/* 2,4,8 or 16 */
		r /= _div;
	}

	/* limit the size */
	switch (rand() % 4) {
	case 0: r &= 0xff;
		break;
	case 1: r &= 0xffff;
		break;
	case 2: r &= 0xffffff;
		break;
	default:
		break;
	}

	return r;
}

/*
 * Generate and munge a 64bit number.
 */
u64 rand64(void)
{
	unsigned long r = 0;

	if (rand_bool()) {
		/* 32-bit ranges. */
		r = rand32();

	} else {
		/* 33:64-bit ranges. */
		switch (rand() % 7) {
		case 0:	r = rand_single_bit(64);
			break;
		case 1:	r = randbits(64);
			break;
		case 2:	r = rand32() | rand32() << 31;
			break;
		case 3:	r = taviso();
			break;
		case 4:	r = rand8x8();
			break;
		case 5:	r = rept8(8);
			break;
		/* Sometimes pick a not-so-random number. */
		case 6:	return get_interesting_value();
		default:
			break;
		}

		/* limit the size */
		switch (rand() % 4) {
		case 0: r &= 0x000000ffffffffffULL;
			break;
		case 1: r &= 0x0000ffffffffffffULL;
			break;
		case 2: r &= 0x00ffffffffffffffULL;
			break;
		default:
			break;
		}
	}

	/* Sometimes invert the generated number. */
	if (rand_bool())
		r = ~r;

	/* increase distribution in MSB */
	if ((rand() % 10)) {
		unsigned int i;
		unsigned int rounds;

		rounds = rand() % 4;
		for (i = 0; i < rounds; i++)
			r |= (1L << ((__WORDSIZE - 1) - (rand() % 8)));
	}

	/* randomly flip sign bit. */
	if (rand_bool())
		r |= (1L << (__WORDSIZE - 1));

	return r;
}
