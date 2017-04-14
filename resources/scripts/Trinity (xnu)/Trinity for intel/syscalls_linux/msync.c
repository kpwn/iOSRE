/*
 * SYSCALL_DEFINE3(msync, unsigned long, start, size_t, len, int, flags)
 */
#include "sanitise.h"

#define MS_ASYNC        1               /* Sync memory asynchronously.  */
#define MS_SYNC         4               /* Synchronous memory sync.  */
#define MS_INVALIDATE   2               /* Invalidate the caches.  */

struct syscall syscall_msync = {
	.name = "msync",
	.num_args = 3,
	.arg1name = "start",
	.arg1type = ARG_ADDRESS,
	.arg2name = "len",
	.arg2type = ARG_LEN,
	.arg3name = "flags",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 3,
		.values = { MS_ASYNC, MS_INVALIDATE, MS_SYNC },
	},
	.group = GROUP_VM,
};
