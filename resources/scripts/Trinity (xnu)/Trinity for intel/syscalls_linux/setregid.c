/*
 * SYSCALL_DEFINE2(setregid, gid_t, rgid, gid_t, egid)
 */
#include "sanitise.h"

struct syscall syscall_setregid = {
	.name = "setregid",
	.num_args = 2,
	.arg1name = "rgid",
	.arg2name = "egid",
};
