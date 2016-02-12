/*
 * SYSCALL_DEFINE3(shmget, key_t, key, size_t, size, int, shmflg)
 */

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sanitise.h"

static void post_shmget(int syscallret)
{
	if (syscallret == -1)
		return;

	shmctl(syscallret, IPC_RMID, 0);
}

struct syscall syscall_shmget = {
	.name = "shmget",
	.num_args = 3,
	.arg1name = "key",
	.arg2name = "size",
	.arg2type = ARG_LEN,
	.arg3name = "shmflg",
	.post = post_shmget,
};
