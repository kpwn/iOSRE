/*
 * SYSCALL_DEFINE5(get_mempolicy, int __user *, policy,
	unsigned long __user *, nmask, unsigned long, maxnode,
	unsigned long, addr, unsigned long, flags)
 */

#define MPOL_F_NODE     (1<<0)  /* return next IL mode instead of node mask */
#define MPOL_F_ADDR     (1<<1)  /* look up vma using address */
#define MPOL_F_MEMS_ALLOWED (1<<2) /* return allowed memories */

#include "sanitise.h"

struct syscall syscall_get_mempolicy = {
	.name = "get_mempolicy",
	.num_args = 5,
	.arg1name = "policy",
	.arg1type = ARG_ADDRESS,
	.arg2name = "nmask",
	.arg2type = ARG_ADDRESS,
	.arg3name = "maxnode",
	.arg3type = ARG_RANGE,
	.low3range = 0,
	.hi3range = 1 << 9,	/* 1 << CONFIG_NODES_SHIFT */
	.arg4name = "addr",
	.arg4type = ARG_ADDRESS,
	.arg5name = "flags",
	.arg5type = ARG_LIST,
	.arg5list = {
		.num = 3,
		.values = { MPOL_F_NODE, MPOL_F_ADDR, MPOL_F_MEMS_ALLOWED },
	},
	.rettype = RET_ZERO_SUCCESS,
};
