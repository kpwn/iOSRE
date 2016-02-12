/*
 * SYSCALL_DEFINE4(tee, int, fdin, int, fdout, size_t, len, unsigned int, flags)
 */
#include <stdlib.h>
#include "sanitise.h"
#include "shm.h"

# define SPLICE_F_MOVE          1       /* Move pages instead of copying.  */
# define SPLICE_F_NONBLOCK      2       /* Don't block on the pipe splicing
                                           (but we may still block on the fd
                                           we splice from/to).  */
# define SPLICE_F_MORE          4       /* Expect more data.  */
# define SPLICE_F_GIFT          8       /* Pages passed in are a gift.  */

static void sanitise_tee(int childno)
{
	if ((rand() % 10) > 0) {
		shm->a1[childno] = shm->pipe_fds[rand() % MAX_PIPE_FDS];
		shm->a2[childno] = shm->pipe_fds[rand() % MAX_PIPE_FDS];
	}
}

struct syscall syscall_tee = {
	.name = "tee",
	.num_args = 4,
	.arg1name = "fdin",
	.arg1type = ARG_FD,
	.arg2name = "fdout",
	.arg2type = ARG_FD,
	.arg3name = "len",
	.arg3type = ARG_LEN,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 4,
		.values = { SPLICE_F_MOVE, SPLICE_F_NONBLOCK, SPLICE_F_MORE, SPLICE_F_GIFT },
	},
	.sanitise = sanitise_tee,
	.flags = NEED_ALARM,
};
