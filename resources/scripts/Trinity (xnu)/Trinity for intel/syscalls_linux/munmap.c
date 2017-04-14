/*
 * SYSCALL_DEFINE2(munmap, unsigned long, addr, size_t, len)
 */
#include "arch.h"	// page_size
#include "sanitise.h"
#include "shm.h"

static void sanitise_munmap(int childno)
{
	shm->a2[childno] = page_size;
}

struct syscall syscall_munmap = {
	.name = "munmap",
	.num_args = 2,
	.arg1name = "addr",
	.arg1type = ARG_NON_NULL_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.group = GROUP_VM,
	.sanitise = sanitise_munmap,

	/* For now, disable this syscall entirely. We segfault when
	   we try to unmap things we need. Resurrect this when we
	   have proper tracking of mappings from other syscalls. */
	.flags = AVOID_SYSCALL,
};
