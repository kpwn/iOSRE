/*
 * SYSCALL_DEFINE1(setuid, uid_t, uid)
 */
#include "random.h"
#include "shm.h"
#include "sanitise.h"

static void sanitise_setuid(int childno)
{
	shm->a1[childno] = rand32();
}

struct syscall syscall_setuid = {
	.name = "setuid",
	.num_args = 1,
	.arg1name = "uid",
	.sanitise = sanitise_setuid,
};
