#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>

#include "trinity.h"	// page_size
#include "arch.h"	// KERNEL_ADDR etc
#include "log.h"
#include "random.h"
#include "sanitise.h"
#include "maps.h"
#include "shm.h"
#include "tables.h"

/*
static bool within_page(void *addr, void *check)
{
	if (addr == check)
		return TRUE;
	if ((addr > check) && (addr < (check + page_size)))
		return TRUE;
	return FALSE;
}

bool validate_address(void *addr)
{
	if (within_page(addr, shm) == TRUE)
		return FALSE;
	if (within_page(addr, page_rand) == TRUE)
		return FALSE;
	if (within_page(addr, page_zeros) == TRUE)
		return FALSE;
	if (within_page(addr, page_0xff) == TRUE)
		return FALSE;
	if (within_page(addr, page_allocs) == TRUE)
		return FALSE;

	return TRUE;
}
*/

static void * _get_address(unsigned char null_allowed)
{
	int i;
	void *addr = NULL;

	if (null_allowed == TRUE)
		i = rand() % 9;
	else
		i = (rand() % 8) + 1;


	switch (i) {
	case 0: addr = NULL;
		break;
	case 1:	addr = (void *) KERNEL_ADDR;
		break;
	case 2:	addr = page_zeros;
		break;
	case 3:	addr = page_0xff;
		break;
	case 4:	addr = page_rand;
		break;
	case 5: addr = page_allocs;
		break;
	case 6:	addr = (void *)(unsigned long)rand64();
		break;
	case 7: addr = get_map();
		break;
	case 8: addr = malloc(page_size * 2);
		// FIXME: We leak this. This is the address we need to store for later
		// freeing, not the potentially munged version below.
		// tricky. We want to hand the munged version out too, so we might end up
		// having to split this into alloc_address / get_address.
		break;
	default:
		BUG("unreachable!\n");
		break;
	}

	/*
	 * Most of the time, we just return the address we got above unmunged.
	 * But sometimes, we return an address just before the end of the page.
	 * The idea here is that we might see some bugs that are caused by page boundary failures.
	 */
	i = rand() % 100;
	switch (i) {
	case 0:	addr += (page_size - sizeof(char));
		break;
	case 1:	addr += (page_size - sizeof(int));
		break;
	case 2:	addr += (page_size - sizeof(long));
		break;
	case 3:	addr += (page_size / 2);
		break;
	case 4 ... 99:
	default: break;
	}

	return addr;
}

void * get_address(void)
{
	return _get_address(TRUE);
}

void * get_non_null_address(void)
{
	return _get_address(FALSE);
}


unsigned long find_previous_arg_address(unsigned int argnum, unsigned int call, int childno)
{
	unsigned long addr = 0;

	if (argnum > 1)
		if ((syscalls[call].entry->arg1type == ARG_ADDRESS) ||
		    (syscalls[call].entry->arg1type == ARG_NON_NULL_ADDRESS))
			addr = shm->a1[childno];

	if (argnum > 2)
		if ((syscalls[call].entry->arg2type == ARG_ADDRESS) ||
		    (syscalls[call].entry->arg2type == ARG_NON_NULL_ADDRESS))
			addr = shm->a2[childno];

	if (argnum > 3)
		if ((syscalls[call].entry->arg3type == ARG_ADDRESS) ||
		    (syscalls[call].entry->arg3type == ARG_NON_NULL_ADDRESS))
			addr = shm->a3[childno];

	if (argnum > 4)
		if ((syscalls[call].entry->arg4type == ARG_ADDRESS) ||
		    (syscalls[call].entry->arg4type == ARG_NON_NULL_ADDRESS))
			addr = shm->a4[childno];

	if (argnum > 5)
		if ((syscalls[call].entry->arg5type == ARG_ADDRESS) ||
		    (syscalls[call].entry->arg5type == ARG_NON_NULL_ADDRESS))
			addr = shm->a5[childno];

	return addr;
}


/*
 * iovec's are just special cases of the ARG_ADDRESS's
 */
struct iovec * alloc_iovec(unsigned int num)
{
	struct iovec *iov;
	unsigned int i;

	iov = malloc(num * sizeof(struct iovec));
	if (iov != NULL) {
		for (i = 0; i < num; i++) {
			iov[i].iov_base = malloc(page_size);
			iov[i].iov_len = page_size;
		}
	}
	return iov;
}
