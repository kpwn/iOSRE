#ifndef _TRINITY_SYSCALL_H
#define _TRINITY_SYSCALL_H 1

#include "types.h"

enum argtype {
	ARG_UNDEFINED = 0,
	ARG_RANDOM_LONG = 1,
	ARG_FD = 2,
	ARG_LEN = 3,
	ARG_ADDRESS = 4,
	ARG_MODE_T = 5,
	ARG_NON_NULL_ADDRESS = 6,
	ARG_PID = 7,
	ARG_RANGE = 8,
	ARG_OP = 9,
	ARG_LIST = 10,
	ARG_RANDPAGE = 11,
	ARG_CPU = 12,
	ARG_PATHNAME = 13,
	ARG_IOVEC = 14,
	ARG_IOVECLEN = 15,
	ARG_SOCKADDR = 16,
	ARG_SOCKADDRLEN = 17,
};

struct arglist {
	unsigned int num;
	unsigned int values[32];
};

struct syscall {
	void (*sanitise)(int childno);
	void (*post)(int);
	int (*init)(void);

	unsigned int number;
	unsigned int active_number;
	const char name[80];
	const unsigned int num_args;
	unsigned int flags;

	const enum argtype arg1type;
	const enum argtype arg2type;
	const enum argtype arg3type;
	const enum argtype arg4type;
	const enum argtype arg5type;
	const enum argtype arg6type;

	const char *arg1name;
	const char *arg2name;
	const char *arg3name;
	const char *arg4name;
	const char *arg5name;
	const char *arg6name;

	/* FIXME: At some point, if we grow more type specific parts here,
	 * it may be worth union-ising this
	 */

	/* ARG_RANGE */
	const unsigned int low1range, hi1range;
	const unsigned int low2range, hi2range;
	const unsigned int low3range, hi3range;
	const unsigned int low4range, hi4range;
	const unsigned int low5range, hi5range;
	const unsigned int low6range, hi6range;

	/* ARG_OP / ARG_LIST */
	const struct arglist arg1list;
	const struct arglist arg2list;
	const struct arglist arg3list;
	const struct arglist arg4list;
	const struct arglist arg5list;
	const struct arglist arg6list;

	const unsigned int group;
	const int rettype;
};

#define RET_BORING		-1
#define RET_NONE		0
#define RET_ZERO_SUCCESS	1
#define RET_FD			2
#define RET_KEY_SERIAL_T	3
#define RET_PID_T		4
#define RET_PATH		5
#define RET_NUM_BYTES		6
#define RET_GID_T		7
#define RET_UID_T		8

#define GROUP_NONE	0
#define GROUP_VM	1
#define GROUP_VFS	2

struct syscalltable {
	struct syscall *entry;
};

#define CAPABILITY_CHECK (1<<0)
#define AVOID_SYSCALL (1<<1)
#define NI_SYSCALL (1<<2)
#define BORING (1<<3)
#define ACTIVE (1<<4)
#define NEED_ALARM (1<<5)
#define TO_BE_DEACTIVATED (1<<6)

long syscall32(unsigned int call, unsigned long a1, unsigned long a2, unsigned long a3, unsigned long a4, unsigned long a5, unsigned long a6);

#endif	/* _TRINITY_SYSCALL_H */
