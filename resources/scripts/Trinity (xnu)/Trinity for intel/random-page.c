#include <stdio.h>
#include <stdlib.h>

#include "arch.h"	// page_size
#include "random.h"
#include "sanitise.h"	// get_address
#include "maps.h"
#include "log.h"	// For BUG

static void fabricate_onepage_struct(char *page)
{
	unsigned int i;
	void **ptr;

	for (i = 0; i < page_size; ) {
		ptr = (void*)&page[i];
		switch (rand() % 4) {
		case 0:
			i += sizeof(unsigned int);
			if (i > page_size)
				return;
			*(unsigned int *)ptr = rand32();
			break;
		case 1:
			i += sizeof(unsigned long);
			if (i > page_size)
				return;
			*(unsigned long *)ptr = rand64();
			break;
		case 2:
			i += sizeof(void *);
			if (i > page_size)
				return;
			*ptr = get_address();
			break;
		case 3:
			i += sizeof(unsigned int);
			if (i > page_size)
				return;
			*(unsigned int *)ptr = rand() % page_size;
			break;
		default:
			BUG("unreachable!\n");
			return;
		}
	}
}

void generate_random_page(char *page)
{
	unsigned int i;
	unsigned int p = 0;

	switch (rand() % 8) {
	/* return a page of complete trash */
	case 0:	/* bytes */
		for (i = 0; i < page_size; )
			page[i++] = (unsigned char)rand();
		return;

	case 1:	/* words */
		for (i = 0; i < (page_size / 2); ) {
			page[i++] = 0;
			page[i++] = (unsigned char)rand();
		}
		return;

	case 2:	/* ints */
		for (i = 0; i < (page_size / 4); ) {
			page[i++] = 0;
			page[i++] = 0;
			page[i++] = 0;
			page[i++] = (unsigned char)rand();
		}
		return;

	/* return a page that looks kinda like a struct */
	case 3:	fabricate_onepage_struct(page);
		return;

	/* return a page of unicode nonsense. */
	case 4:	gen_unicode_page(page);
		return;

	/* page of 0's and 1's. */
	case 5:
		for (i = 0; i < page_size; )
			page[i++] = (unsigned char)rand_bool();
		return;

	/* page full of format strings. */
	case 6:
		for (i = 0; i < page_size; ) {
			page[i++] = '%';
			switch (rand_bool()) {
			case 0:	page[i++] = 'd';
				break;
			case 1:	page[i++] = 's';
				break;
			default: break;
			}
		}
		page_size = getpagesize();	// Hack for clang 3.3 false positive.
		page[rand() % page_size] = 0;
		return;

	/* ascii representation of a random number */
	case 7:
		switch (rand() % 3) {
		case 0:
			switch (rand() % 3) {
			case 0:	p = sprintf(page_rand, "%lu", (unsigned long) rand64());
				break;
			case 1:	p = sprintf(page_rand, "%ld", (unsigned long) rand64());
				break;
			case 2:	p = sprintf(page_rand, "%lx", (unsigned long) rand64());
				break;
			default: break;
			}
			break;

		case 1:
			switch (rand() % 3) {
			case 0:	p = sprintf(page_rand, "%u", (unsigned int) rand64());
				break;
			case 1:	p = sprintf(page_rand, "%d", (int) rand64());
				break;
			case 2:	p = sprintf(page_rand, "%x", (int) rand64());
				break;
			default: break;
			}
			break;

		case 2:
			switch (rand() % 3) {
			case 0:	p = sprintf(page_rand, "%u", (unsigned char) rand64());
				break;
			case 1:	p = sprintf(page_rand, "%d", (char) rand64());
				break;
			case 2:	p = sprintf(page_rand, "%x", (char) rand64());
				break;
			default: break;
			}
			break;

		default: break;
		}

		page_rand[p] = 0;

		break;

	default:
		BUG("unreachable!\n");
		return;
	}
}
