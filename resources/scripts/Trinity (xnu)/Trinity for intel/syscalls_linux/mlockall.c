/*
 * SYSCALL_DEFINE1(mlockall, int, flags)
 */
#include <stdlib.h>
#include "sanitise.h"
#include "random.h"
#include "shm.h"

#define MCL_CURRENT     1
#define MCL_FUTURE      2

static void sanitise_mlockall(int childno)
{
	if (shm->a1[childno] != 0)
		return;

	if (rand_bool())
		shm->a1[childno] = MCL_CURRENT;
	else
		shm->a1[childno] = MCL_FUTURE;
}


struct syscall syscall_mlockall = {
	.name = "mlockall",
	.num_args = 1,
	.arg1name = "flags",
	.arg1type = ARG_LIST,
	.arg1list = {
		.num = 2,
		.values = { MCL_CURRENT, MCL_FUTURE },
	},
	.group = GROUP_VM,
	.sanitise = sanitise_mlockall,
};
