/*
 * SYSCALL_DEFINE1(acct, const char __user *, name)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_acct = {
	.name = "acct",
	.num_args = 1,
	.arg1name = "name",
	.arg1type = ARG_ADDRESS,
};
