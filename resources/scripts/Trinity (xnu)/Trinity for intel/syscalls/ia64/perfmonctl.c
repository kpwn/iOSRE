/*
    sys_perfmonctl (int fd, int cmd, void __user *arg, int count)
 */

#include <unistd.h>
#include <asm/perfmon.h>

#ifndef PFM_CREATE_EVTSETS
#define PFM_CREATE_EVTSETS 0
#endif

#ifndef PFM_DELETE_EVTSETS
#define PFM_DELETE_EVTSETS 0
#endif

#ifndef PFM_GETINFO_EVTSETS
#define PFM_GETINFO_EVTSETS 0
#endif

#include "trinity.h"
#include "sanitise.h"

struct syscall syscall_perfmonctl = {
	.name = "perfmonctl",
	.num_args = 4,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "cmd",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 12,
		.values = { PFM_CREATE_CONTEXT, PFM_WRITE_PMCS, PFM_WRITE_PMDS,
			    PFM_READ_PMDS, PFM_START, PFM_STOP, PFM_LOAD_CONTEXT,
			    PFM_UNLOAD_CONTEXT, PFM_RESTART, PFM_CREATE_EVTSETS,
			    PFM_DELETE_EVTSETS, PFM_GETINFO_EVTSETS },
	},
	.arg3name = "arg",
	.arg3type = ARG_ADDRESS,
	.arg4name = "count",
};

