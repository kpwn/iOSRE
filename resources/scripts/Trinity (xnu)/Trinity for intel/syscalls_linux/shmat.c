/*
 * SYSCALL_DEFINE3(shmat, int, shmid, char __user *, shmaddr, int, shmflg)
 */
#include "sanitise.h"

struct syscall syscall_shmat = {
	.name = "shmat",
	.num_args = 3,
	.arg1name = "shmid",
	.arg2name = "shmaddr",
	.arg2type = ARG_ADDRESS,
	.arg3name = "shmflg",
};
