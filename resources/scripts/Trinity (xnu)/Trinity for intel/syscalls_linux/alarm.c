/*
 * SYSCALL_DEFINE1(alarm, unsigned int, seconds)
 *
 * returns  the number of seconds remaining until any previously scheduled alarm was due to be delivered,
 *  or zero if there was no previously scheduled
 */
#include "sanitise.h"

struct syscall syscall_alarm = {
	.flags = AVOID_SYSCALL,	/* we rely on a useful alarm for every syscall. */
	.name = "alarm",
	.num_args = 1,
	.arg1name = "seconds",
};
