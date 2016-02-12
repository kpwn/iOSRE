/*
 * SYSCALL_DEFINE(sync_file_range)(int fd, loff_t offset, loff_t nbytes, unsigned int flags)
 * SYSCALL_DEFINE(sync_file_range2)(int fd, unsigned int flags, loff_t offset, loff_t nbytes)
 */
#include <linux/fs.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "arch.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"
#include "tables.h"

struct syscall syscall_sync_file_range;

static void sanitise_sync_file_range(int childno)
{
	unsigned int call = shm->syscallno[childno];
	struct syscall *syscall_entry = syscalls[call].entry;
	long endbyte;
	loff_t nbytes;
	loff_t off;

retry:
	off = rand64() & 0x0fffffffffffffff;
	nbytes = rand64() & 0x0fffffffffffffff;
	endbyte = off + nbytes;
	if (endbyte < off)
		goto retry;

	if (off >= (0x100000000LL << PAGE_SHIFT))
		goto retry;

	if (strcmp("sync_file_range2", syscall_entry->name) == 0) {
		shm->a2[childno] = off;
		shm->a3[childno] = nbytes;
	} else {
		shm->a3[childno] = off;
		shm->a4[childno] = nbytes;
	}
}

struct syscall syscall_sync_file_range = {
	.name = "sync_file_range",
	.num_args = 4,
	.sanitise = sanitise_sync_file_range,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "offset",
	.arg3name = "nbytes",
	.arg3type = ARG_LEN,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
        .arg4list = {
		.num = 3,
		.values = { SYNC_FILE_RANGE_WAIT_BEFORE, SYNC_FILE_RANGE_WRITE, SYNC_FILE_RANGE_WAIT_AFTER },
        },
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};

/*
 * ARM & PowerPC have different argument order.
 * See edd5cd4a9424f22b0fa08bef5e299d41befd5622 in kernel tree.
 */
struct syscall syscall_sync_file_range2 = {
	.name = "sync_file_range2",
	.num_args = 4,
	.sanitise = sanitise_sync_file_range,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "flags",
	.arg2type = ARG_LIST,
        .arg2list = {
		.num = 3,
		.values = { SYNC_FILE_RANGE_WAIT_BEFORE, SYNC_FILE_RANGE_WRITE, SYNC_FILE_RANGE_WAIT_AFTER },
	},
	.arg3name = "offset",
	.arg4name = "nbytes",
	.arg4type = ARG_LEN,
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
};
