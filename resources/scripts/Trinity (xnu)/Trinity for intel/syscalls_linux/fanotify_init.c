/*
 * SYSCALL_DEFINE2(fanotify_init, unsigned int, flags, unsigned int, event_f_flags)
 */

#define FAN_CLOEXEC		0x00000001
#define FAN_NONBLOCK		0x00000002
#define FAN_CLASS_NOTIF		0x00000000
#define FAN_CLASS_CONTENT	0x00000004
#define FAN_CLASS_PRE_CONTENT	0x00000008
#define FAN_UNLIMITED_QUEUE	0x00000010
#define FAN_UNLIMITED_MARKS	0x00000020

#include "sanitise.h"

struct syscall syscall_fanotify_init = {
	.name = "fanotify_init",
	.num_args = 2,
	.arg1name = "flags",
	.arg1type = ARG_LIST,
	.arg1list = {
		.num = 7,
		.values = { FAN_CLOEXEC , FAN_NONBLOCK, FAN_UNLIMITED_QUEUE , FAN_UNLIMITED_MARKS,
				FAN_CLASS_NOTIF, FAN_CLASS_CONTENT, FAN_CLASS_PRE_CONTENT
		},
	},
	.arg2name = "event_f_flags",
	.flags = CAPABILITY_CHECK,
	.rettype = RET_FD,
	.group = GROUP_VFS,
};
