/*
 * SYSCALL_DEFINE(pread64)(unsigned int fd, char __user *buf, size_t count, loff_t pos)
 */
#include "random.h"
#include "sanitise.h"
#include "shm.h"

static void sanitise_pread64(int childno)
{

retry_pos:
	if ((int) shm->a4[childno] < 0) {
		shm->a4[childno] = rand64();
		goto retry_pos;
	}
}

struct syscall syscall_pread64 = {
	.name = "pread64",
	.num_args = 4,
	.sanitise = sanitise_pread64,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "buf",
	.arg2type = ARG_ADDRESS,
	.arg3name = "count",
	.arg3type = ARG_LEN,
	.arg4name = "pos",
	.flags = NEED_ALARM,
};
