/*
 * SYSCALL_DEFINE5(mount, char __user *, dev_name, char __user *, dir_name,
	 char __user *, type, unsigned long, flags, void __user *, data)
 */

#include <linux/fs.h>
#include "sanitise.h"
#include "compat.h"

//TODO: fill out 'type' with something random from /proc/filesystems

struct syscall syscall_mount = {
	.name = "mount",
	.num_args = 5,
	.arg1name = "dev_name",
	.arg1type = ARG_PATHNAME,
	.arg2name = "dir_name",
	.arg2type = ARG_PATHNAME,
	.arg3name = "type",
	.arg3type = ARG_ADDRESS,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 29,
		.values = {
			MS_RDONLY, MS_NOSUID, MS_NODEV, MS_NOEXEC,
			MS_SYNCHRONOUS, MS_REMOUNT, MS_MANDLOCK, MS_DIRSYNC,
			MS_NOATIME, MS_NODIRATIME, MS_BIND, MS_MOVE,
			MS_REC, MS_VERBOSE, MS_SILENT, MS_POSIXACL,
			MS_UNBINDABLE, MS_PRIVATE, MS_SLAVE, MS_SHARED,
			MS_RELATIME, MS_KERNMOUNT, MS_I_VERSION, MS_STRICTATIME,
			MS_SNAP_STABLE, MS_NOSEC, MS_BORN, MS_ACTIVE,
			MS_NOUSER, },
	},
	.arg5name = "data",
	.arg5type = ARG_ADDRESS,
	.group = GROUP_VFS,
};
