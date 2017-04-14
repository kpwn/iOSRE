/*
 *
 * SYSCALL_DEFINE1(newuname, struct new_utsname __user *, name)
 */
#include "sanitise.h"

struct syscall syscall_newuname = {
	.name = "newuname",
	.num_args = 1,
	.arg1name = "name",
	.arg1type = ARG_ADDRESS,
};
