/*
 * SYSCALL_DEFINE2(capset, cap_user_header_t, header, const cap_user_data_t, data)
 *
 * On success, zero is returned.
 * On error, -1 is returned, and errno is set appropriately.
 */
#include "sanitise.h"

struct syscall syscall_capset = {
	.name = "capset",
	.num_args = 2,
	.arg1name = "header",
	.arg2name = "data",
	.rettype = RET_ZERO_SUCCESS,
};
