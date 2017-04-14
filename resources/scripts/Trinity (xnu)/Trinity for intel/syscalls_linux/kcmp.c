/*
 * SYSCALL_DEFINE5(kcmp, pid_t, pid1, pid_t, pid2, int, type,
 *               unsigned long, idx1, unsigned long, idx2)
 *
 */
#include "sanitise.h"
#include "compat.h"

struct syscall syscall_kcmp = {
	.name = "kcmp",
	.num_args = 5,
	.arg1name = "pid1",
	.arg1type = ARG_PID,
	.arg2name = "pid2",
	.arg2type = ARG_PID,
	.arg3name = "type",
	.arg3type = ARG_OP,
	.arg3list = {
		.num = KCMP_TYPES,
		.values = { KCMP_FILE, KCMP_VM, KCMP_FILES, KCMP_FS, KCMP_SIGHAND, KCMP_IO, KCMP_SYSVSEM },
	},
	.arg4name = "idx1",
	.arg5name = "idx2",
};
