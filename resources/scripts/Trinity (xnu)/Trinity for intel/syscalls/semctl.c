/*
 * SYSCALL_DEFINE(semctl)(int semid, int semnum, int cmd, union semun arg)
 */
#include "sanitise.h"

struct syscall syscall_semctl = {
	.name = "semctl",
	.num_args = 4,
	.arg1name = "semid",
	.arg2name = "semnum",
	.arg3name = "cmd",
	.arg4name = "arg",
};
