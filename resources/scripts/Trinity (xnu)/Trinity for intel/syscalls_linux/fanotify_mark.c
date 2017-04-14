/*
 * SYSCALL_DEFINE(fanotify_mark)(int fanotify_fd, unsigned int flags,
	__u64 mask, int dfd, const char  __user * pathname)
 */
#include <stdlib.h>
#include "sanitise.h"
#include "shm.h"

/* flags used for fanotify_modify_mark() */
#define FAN_MARK_ADD            0x00000001
#define FAN_MARK_REMOVE         0x00000002
#define FAN_MARK_DONT_FOLLOW    0x00000004
#define FAN_MARK_ONLYDIR        0x00000008
#define FAN_MARK_MOUNT          0x00000010
#define FAN_MARK_IGNORED_MASK   0x00000020
#define FAN_MARK_IGNORED_SURV_MODIFY    0x00000040
#define FAN_MARK_FLUSH          0x00000080

#define FAN_ACCESS              0x00000001      /* File was accessed */
#define FAN_MODIFY              0x00000002      /* File was modified */
#define FAN_CLOSE_WRITE         0x00000008      /* Writtable file closed */
#define FAN_CLOSE_NOWRITE       0x00000010      /* Unwrittable file closed */
#define FAN_OPEN                0x00000020      /* File was opened */

#define FAN_Q_OVERFLOW          0x00004000      /* Event queued overflowed */

#define FAN_OPEN_PERM           0x00010000      /* File open in perm check */
#define FAN_ACCESS_PERM         0x00020000      /* File accessed in perm check */

#define FAN_ONDIR               0x40000000      /* event occurred against dir */

#define FAN_EVENT_ON_CHILD      0x08000000      /* interested in child events */
#define FAN_CLOSE               (FAN_CLOSE_WRITE | FAN_CLOSE_NOWRITE) /* close */

static void sanitise_fanotify_mark(int childno)
{
	unsigned int flagvals[5] = { FAN_MARK_DONT_FOLLOW, FAN_MARK_ONLYDIR, FAN_MARK_MOUNT,
				    FAN_MARK_IGNORED_MASK, FAN_MARK_IGNORED_SURV_MODIFY };

	unsigned int i;
	unsigned int numflags = rand() % 5;

	// set additional flags
	for (i = 0; i < numflags; i++)
		shm->a2[childno] |= flagvals[i];

	// Set mask
	shm->a3[childno] &= 0xffffffff;
}

struct syscall syscall_fanotify_mark = {
	.name = "fanotify_mark",
	.num_args = 5,
	.arg1name = "fanotify_fd",
	.arg1type = ARG_FD,
	.arg2name = "flags",
	.arg2type = ARG_OP,
	.arg2list = {
		.num = 3,
		.values = { FAN_MARK_ADD, FAN_MARK_REMOVE, FAN_MARK_FLUSH },
	},
	.arg3name = "mask",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 7,
		.values = { FAN_ACCESS, FAN_MODIFY, FAN_CLOSE, FAN_OPEN,
			    FAN_OPEN_PERM, FAN_ACCESS_PERM,
			    FAN_EVENT_ON_CHILD },
	},
	.arg4name = "dfd",
	.arg4type = ARG_FD,
	.arg5name = "pathname",
	.arg5type = ARG_PATHNAME,
	.sanitise = sanitise_fanotify_mark,
	.rettype = RET_ZERO_SUCCESS,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
