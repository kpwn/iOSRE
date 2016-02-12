/*
 * SYSCALL_DEFINE6(mmap, unsigned long, addr, unsigned long, len,
	unsigned long, prot, unsigned long, flags,
	unsigned long, fd, unsigned long, off)
 */
#include <stdlib.h>
#include <asm/mman.h>
#include "trinity.h"	// page_size
#include "sanitise.h"
#include "shm.h"
#include "arch.h"
#include "compat.h"

#ifdef __x86_64__
#define NUM_FLAGS 13
#else
#define NUM_FLAGS 12
#endif

// need this to actually get MAP_UNINITIALIZED defined
#define CONFIG_MMAP_ALLOW_UNINITIALIZED

void sanitise_mmap(int childno)
{
	unsigned int i;
	unsigned int flagvals[NUM_FLAGS] = { MAP_FIXED, MAP_ANONYMOUS,
			MAP_GROWSDOWN, MAP_DENYWRITE, MAP_EXECUTABLE, MAP_LOCKED,
			MAP_NORESERVE, MAP_POPULATE, MAP_NONBLOCK, MAP_STACK,
			MAP_HUGETLB, MAP_UNINITIALIZED,
#ifdef __x86_64__
			MAP_32BIT,
#endif
	};
	unsigned int numflags = rand() % NUM_FLAGS;

	/* Don't actually set a hint right now, in case we give out
	   something that we don't want changed.  One day, we'll recycle
	   mappings from mmap results and the like here instead.
	   Right now, ARG_ADDRESS is a bad choice, as it causes page_rand()
	   to be remapped as unwritable/unreadable, and then we segfault */
	shm->a1[childno] = 0;

	shm->a2[childno] = page_size;
	if (shm->a2[childno] == 0)
		shm->a2[childno] = page_size;


	// set additional flags
	for (i = 0; i < numflags; i++)
		shm->a4[childno] |= flagvals[rand() % NUM_FLAGS];

	/* no fd if anonymous mapping. */
	if (shm->a4[childno] & MAP_ANONYMOUS)
		shm->a5[childno] = -1;

	/* page align non-anonymous mappings. */
	if (shm->a4[childno] & MAP_ANONYMOUS)
		shm->a6[childno] &= PAGE_MASK;
	else
		shm->a6[childno] = 0;

}

struct syscall syscall_mmap = {
	.name = "mmap",
	.num_args = 6,
	.sanitise = sanitise_mmap,
	.arg1name = "addr",
	.arg1type = ARG_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.arg3name = "prot",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 4,
		.values = { PROT_READ, PROT_WRITE, PROT_EXEC, PROT_SEM },
	},
	.arg4name = "flags",
	.arg4type = ARG_OP,
	.arg4list = {
		.num = 2,
		.values = { MAP_SHARED, MAP_PRIVATE },
	},
	.arg5name = "fd",
	.arg5type = ARG_FD,
	.arg6name = "off",
	.arg6type = ARG_LEN,
	.group = GROUP_VM,
	.flags = NEED_ALARM,
};
