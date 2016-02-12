/*
 * SYSCALL_DEFINE5(remap_file_pages, unsigned long, start, unsigned long, size,
	 unsigned long, prot, unsigned long, pgoff, unsigned long, flags)
 */
#include <asm/mman.h>
#include "trinity.h"	// page_size
#include "arch.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"

static void sanitise_remap_file_pages(int childno)
{

	shm->a1[childno] &= PAGE_MASK;
	shm->a2[childno] &= PAGE_MASK;


retry_size:
	if (shm->a1[childno] + shm->a2[childno] <= shm->a1[childno]) {
		shm->a2[childno] = rand32() & PAGE_MASK;
		goto retry_size;
	}

retry_pgoff:
	if (shm->a4[childno] + (shm->a2[childno] >> PAGE_SHIFT) < shm->a4[childno]) {
		shm->a4[childno] = rand64();
		goto retry_pgoff;
	}

retry_pgoff_bits:
	if (shm->a4[childno] + (shm->a2[childno] >> PAGE_SHIFT) >= (1UL << PTE_FILE_MAX_BITS)) {
		shm->a4[childno] = (shm->a4[childno] >> 1);
		goto retry_pgoff_bits;
	}
}

struct syscall syscall_remap_file_pages = {
	.name = "remap_file_pages",
	.sanitise = sanitise_remap_file_pages,
	.num_args = 5,
	.arg1name = "start",
	.arg1type = ARG_ADDRESS,
	.arg2name = "size",
	.arg2type = ARG_LEN,
	.arg3name = "prot",
	.arg4name = "pgoff",
	.arg5name = "flags",
	.arg5type = ARG_LIST,
	.arg5list = {
		.num = 1,
		.values = { MAP_NONBLOCK },
	},
	.group = GROUP_VM,
};
