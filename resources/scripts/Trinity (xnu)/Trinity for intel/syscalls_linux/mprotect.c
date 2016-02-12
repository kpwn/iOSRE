/*
 * sys_mprotect(unsigned long start, size_t len, unsigned long prot)
 */
#include <asm/mman.h>
#include "trinity.h"	// page_size
#include "arch.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"

static void sanitise_mprotect(int childno)
{
	unsigned long end;

	shm->a1[childno] &= PAGE_MASK;

retry_end:
	end = shm->a1[childno] + shm->a2[childno];
	/* Length must not be zero. */
	if (shm->a2[childno] == 0) {
		shm->a2[childno] = rand64();
		goto retry_end;
	}

	/* End must be after start */
	if (end <= shm->a1[childno]) {
		shm->a2[childno] = rand64();
		goto retry_end;
	}
}

struct syscall syscall_mprotect = {
	.name = "mprotect",
	.num_args = 3,
	.arg1name = "start",
	.arg1type = ARG_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.arg3name = "prot",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 6,
		.values = { PROT_READ, PROT_WRITE, PROT_EXEC, PROT_SEM, PROT_GROWSDOWN, PROT_GROWSUP },
	},
	.sanitise = sanitise_mprotect,
	.group = GROUP_VM,

	.flags = AVOID_SYSCALL,	// hopefully temporary.
};
