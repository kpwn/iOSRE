/*
 * SYSCALL_DEFINE2(mlock, unsigned long, start, size_t, len)
 */
#include <stdlib.h>
#include "shm.h"
#include "sanitise.h"

static void sanitise_mlock(int childno)
{
	if (shm->a2[childno] == 0)
		shm->a2[childno] = 1;	// must be non-null.
}

struct syscall syscall_mlock = {
	.name = "mlock",
	.num_args = 2,
	.arg1name = "addr",
	.arg1type = ARG_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.group = GROUP_VM,
	.sanitise = sanitise_mlock,
};
