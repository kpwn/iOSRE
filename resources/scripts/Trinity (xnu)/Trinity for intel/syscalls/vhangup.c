/*
 * SYSCALL_DEFINE0(vhangup
 */
#include "sanitise.h"

struct syscall syscall_vhangup = {
	.name = "vhangup",
	.num_args = 0,
	.flags = AVOID_SYSCALL, // No args, confuses fuzzer
};
