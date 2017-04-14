/*
 * SYSCALL_DEFINE4(kexec_load, unsigned long, entry, unsigned long, nr_segments,
	struct kexec_segment __user *, segments, unsigned long, flags)
 */

#define KEXEC_ON_CRASH          0x00000001
#define KEXEC_PRESERVE_CONTEXT  0x00000002

#include "sanitise.h"

struct syscall syscall_kexec_load = {
	.name = "kexec_load",
	.num_args = 4,
	.flags = CAPABILITY_CHECK,
	.arg1name = "entry",
	.arg1type = ARG_ADDRESS,
	.arg2name = "nr_segments",
	.arg2type = ARG_LEN,
	.arg3name = "segments",
	.arg3type = ARG_ADDRESS,
	.arg4name = "flags",
	.arg4type = ARG_LIST,
	.arg4list = {
		.num = 2,
		.values = { KEXEC_ON_CRASH, KEXEC_PRESERVE_CONTEXT },
	},
};
