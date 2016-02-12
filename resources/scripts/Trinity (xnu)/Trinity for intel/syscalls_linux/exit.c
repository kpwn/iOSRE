/*
 * SYSCALL_DEFINE1(exit, int, error_code)
 */
#include "sanitise.h"

struct syscall syscall_exit = {
	.name = "exit",
	.num_args = 1,
	.flags = AVOID_SYSCALL, // No args to fuzz, confuses fuzzer
	.arg1name = "error_code",
	.arg1type = ARG_RANDOM_LONG,
};
