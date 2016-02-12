/*
 * SYSCALL_DEFINE5(linkat, int, olddfd, const char __user *, oldname,
	 int, newdfd, const char __user *, newname, int, flags)
 */
#include <fcntl.h>
#include <stdlib.h>
#include "shm.h"
#include "sanitise.h"
#include "compat.h"

static void sanitise_linkat(int childno)
{
	/* .. If oldpath is relative and olddirfd is the special value AT_FDCWD, then oldpath is
	 * interpreted relative to the current working directory of the calling process  */
	if ((rand() % 100) == 0)
		shm->a1[childno] = AT_FDCWD;
}

struct syscall syscall_linkat = {
	.name = "linkat",
	.num_args = 5,
	.arg1name = "olddfd",
	.arg1type = ARG_FD,
	.arg2name = "oldname",
	.arg2type = ARG_PATHNAME,
	.arg3name = "newdfd",
	.arg3type = ARG_FD,
	.arg4name = "newname",
	.arg4type = ARG_PATHNAME,
	.arg5name = "flags",
	.arg5type = ARG_LIST,
	.arg5list = {
		.num = 2,
		.values = { AT_SYMLINK_FOLLOW , AT_EMPTY_PATH },
	},
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
	.sanitise = sanitise_linkat,
};
