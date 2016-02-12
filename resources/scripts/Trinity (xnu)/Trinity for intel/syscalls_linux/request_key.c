/*
 * SYSCALL_DEFINE4(request_key, const char __user *, _type,
	const char __user *, _description,
	const char __user *, _callout_info,
	key_serial_t, destringid)
 */
#include "sanitise.h"

struct syscall syscall_request_key = {
	.name = "request_key",
	.num_args = 4,
	.arg1name = "_type",
	.arg1type = ARG_ADDRESS,
	.arg2name = "_description",
	.arg2type = ARG_ADDRESS,
	.arg3name = "_callout_info",
	.arg3type = ARG_ADDRESS,
	.arg4name = "destringid",
};
