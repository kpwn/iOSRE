/*
 * asmlinkage unsigned long sys_mremap(unsigned long addr,
 *   unsigned long old_len, unsigned long new_len,
 *   unsigned long flags, unsigned long new_addr)
 *
 * This syscall is a bit of a nightmare to fuzz as we -EINVAL all over the place.
 * It might be more useful once we start passing around valid maps instead of just
 * trying random addresses.
 */
#include <linux/mman.h>
#include <stdlib.h>
#include "trinity.h"	// page_size
#include "arch.h"
#include "sanitise.h"
#include "shm.h"

static void sanitise_mremap(int childno)
{
	shm->a1[childno] &= PAGE_MASK;

	if (shm->a4[childno] & MREMAP_FIXED) {
		// Can't be fixed, and maymove.
		shm->a4[childno] &= ~MREMAP_MAYMOVE;

		shm->a3[childno] &= TASK_SIZE - shm->a3[childno];
	}
}

struct syscall syscall_mremap = {
	.name = "mremap",
	.num_args = 5,
	.sanitise = sanitise_mremap,
	.arg1name = "addr",
	.arg1type = ARG_NON_NULL_ADDRESS,
	.arg2name = "old_len",
	.arg2type = ARG_LEN,
	.arg3name = "new_len",
	.arg3type = ARG_LEN,
	.arg4name = "flags",
        .arg4type = ARG_LIST,
        .arg4list = {
		.num = 2,
		.values = { MREMAP_MAYMOVE, MREMAP_FIXED },
        },
	.arg5name = "new_addr",
	.arg5type = ARG_ADDRESS,
	.group = GROUP_VM,
};
