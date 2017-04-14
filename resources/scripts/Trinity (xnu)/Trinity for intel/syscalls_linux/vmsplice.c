/*
 * SYSCALL_DEFINE4(vmsplice, int, fd, const struct iovec __user *, iov,
	 unsigned long, nr_segs, unsigned int, flags)
 */

#include <fcntl.h>
#include <sys/uio.h>
#include <stdlib.h>
#include "sanitise.h"
#include "shm.h"

static void sanitise_vmsplice(int childno)
{
	if ((rand() % 10) > 0)
		shm->a1[childno] = shm->pipe_fds[rand() % MAX_PIPE_FDS];
	shm->a3[childno] = rand() % UIO_MAXIOV;
}

struct syscall syscall_vmsplice = {
	.name = "vmsplice",
	.num_args = 4,
	.sanitise = sanitise_vmsplice,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "iov",
	.arg2type = ARG_IOVEC,
	.arg3name = "nr_segs",
	.arg3type = ARG_IOVECLEN,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 4,
		.values = { SPLICE_F_MOVE, SPLICE_F_NONBLOCK, SPLICE_F_MORE, SPLICE_F_GIFT },
	},
	.group = GROUP_VM,
	.flags = NEED_ALARM,
};
