/*
 * SYSCALL_DEFINE2(capget, cap_user_header_t, header, cap_user_data_t, dataptr)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_capget = {
	.name = "capget",
	.num_args = 2,
	.arg1name = "header",
	.arg2name = "dataptr",
	.rettype = RET_ZERO_SUCCESS,
};
