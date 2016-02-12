/*
 * SYSCALL_DEFINE3(write, unsigned int, fd, const char __user *, buf, size_t, count)
 */
#include <stdlib.h>
#include "maps.h"
#include "sanitise.h"
#include "shm.h"
#include "arch.h"	// page_size

static void sanitise_write(int childno)
{
	shm->a2[childno] = (unsigned long) page_rand;
	if ((rand() % 100) > 50)
		shm->a3[childno] = 1;
	else
		shm->a3[childno] = rand() % page_size;
}

struct syscall syscall_write = {
	.name = "write",
	.num_args = 3,
	.sanitise = sanitise_write,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "buf",
	.arg2type = ARG_ADDRESS,
	.arg3name = "count",
	.arg3type = ARG_LEN,
	.flags = NEED_ALARM,
};
