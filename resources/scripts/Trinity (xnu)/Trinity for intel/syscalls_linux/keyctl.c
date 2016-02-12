/*
 * SYSCALL_DEFINE5(keyctl, int, option, unsigned long, arg2, unsigned long, arg3,
	unsigned long, arg4, unsigned long, arg5)
 */
#include <linux/keyctl.h>
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_keyctl = {
	.name = "keyctl",
	.num_args = 5,
	.arg1name = "cmd",
	.arg1type = ARG_OP,
	.arg1list = {
		.num = 21,
		.values = { KEYCTL_GET_KEYRING_ID, KEYCTL_JOIN_SESSION_KEYRING, KEYCTL_UPDATE, KEYCTL_REVOKE,
			KEYCTL_CHOWN, KEYCTL_SETPERM, KEYCTL_DESCRIBE, KEYCTL_CLEAR,
			KEYCTL_LINK, KEYCTL_UNLINK, KEYCTL_SEARCH, KEYCTL_READ,
			KEYCTL_INSTANTIATE, KEYCTL_NEGATE, KEYCTL_SET_REQKEY_KEYRING, KEYCTL_SET_TIMEOUT,
			KEYCTL_ASSUME_AUTHORITY, KEYCTL_GET_SECURITY, KEYCTL_SESSION_TO_PARENT, KEYCTL_REJECT,
			KEYCTL_INSTANTIATE_IOV },
	},
	.arg2name = "arg2",
	.arg3name = "arg3",
	.arg4name = "arg4",
	.arg5name = "arg5",
};
