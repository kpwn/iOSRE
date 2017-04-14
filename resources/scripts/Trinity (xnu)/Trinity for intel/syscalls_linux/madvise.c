/*
 * SYSCALL_DEFINE3(madvise, unsigned long, start, size_t, len_in, int, behavior)
 */
#include <stdlib.h>
#include <sys/mman.h>
#include "arch.h"	// page_size
#include "sanitise.h"
#include "shm.h"
#include "compat.h"

static void sanitise_madvise(int childno)
{
	shm->a2[childno] = rand() % page_size;
}

struct syscall syscall_madvise = {
	.name = "madvise",
	.num_args = 3,
	.arg1name = "start",
	.arg1type = ARG_NON_NULL_ADDRESS,
	.arg2name = "len_in",
	.arg2type = ARG_LEN,
	.arg3name = "advice",
	.arg3type = ARG_OP,
	.arg3list = {
		.num = 12,
		.values = { MADV_NORMAL, MADV_RANDOM, MADV_SEQUENTIAL, MADV_WILLNEED,
			    MADV_DONTNEED, MADV_REMOVE, MADV_DONTFORK, MADV_DOFORK,
			    MADV_MERGEABLE, MADV_UNMERGEABLE, MADV_HUGEPAGE, MADV_NOHUGEPAGE },
	},
	.group = GROUP_VM,
	.sanitise = sanitise_madvise,
};
