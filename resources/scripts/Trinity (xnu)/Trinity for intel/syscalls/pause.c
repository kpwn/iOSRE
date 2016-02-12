/*
 * SYSCALL_DEFINE0(pause)
 */
#include "sanitise.h"

struct syscall syscall_pause = {
	.name = "pause",
	.num_args = 0,
	.flags = AVOID_SYSCALL, // Boring.  Can cause long sleeps
};
