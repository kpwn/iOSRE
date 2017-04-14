/*
 * SYSCALL_DEFINE3(read, unsigned int, fd, char __user *, buf, size_t, count)
 */
#include <stdlib.h>
#include <string.h>
#include "maps.h"
#include "sanitise.h"
#include "shm.h"
#include "arch.h"

static void sanitise_read(int childno)
{
	shm->a2[childno] = (unsigned long) page_rand;
	shm->a3[childno] = rand() % page_size;
}

struct syscall syscall_read = {
	.name = "read",
	.num_args = 3,
	.sanitise = sanitise_read,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "buf",
	.arg2type = ARG_ADDRESS,
	.arg3name = "count",
	.arg3type = ARG_LEN,
	.flags = NEED_ALARM,
};
