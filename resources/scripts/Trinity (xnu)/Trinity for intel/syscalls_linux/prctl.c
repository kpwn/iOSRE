/*
 * SYSCALL_DEFINE5(prctl, int, option, unsigned long, arg2, unsigned long, arg3,
	 unsigned long, arg4, unsigned long, arg5)
 */
#include "config.h"

#include <stdlib.h>
#include <linux/prctl.h>
#ifdef USE_SECCOMP
#include <linux/seccomp.h>
#endif
#include <sys/prctl.h>

#include "sanitise.h"
#include "net.h"
#include "maps.h"
#include "shm.h"
#include "compat.h"

#define NR_PRCTL_OPTS 28
static int prctl_opts[NR_PRCTL_OPTS] = {
	PR_CAPBSET_READ, PR_CAPBSET_DROP, PR_SET_DUMPABLE, PR_GET_DUMPABLE,
	PR_SET_ENDIAN, PR_GET_ENDIAN, PR_SET_FPEMU, PR_GET_FPEMU, PR_SET_FPEXC,
	PR_GET_FPEXC, PR_SET_KEEPCAPS, PR_GET_KEEPCAPS, PR_SET_NAME,
	PR_GET_NAME, PR_SET_PDEATHSIG, PR_GET_PDEATHSIG, PR_SET_SECCOMP,
	PR_GET_SECCOMP, PR_SET_SECUREBITS, PR_GET_SECUREBITS, PR_SET_TIMING,
	PR_GET_TIMING, PR_SET_TSC, PR_GET_TSC, PR_SET_UNALIGN, PR_GET_UNALIGN,
	PR_MCE_KILL, PR_MCE_KILL_GET,
};

/* We already got a generic_sanitise at this point */
void sanitise_prctl(int childno)
{
	int option = prctl_opts[rand() % NR_PRCTL_OPTS];

// For now, just do SECCOMP, the other options need some attention.
option = PR_SET_SECCOMP;

	/* Also allow crap by small chance */
	if (rand() % 100 != 0)
		shm->a1[childno] = option;

	switch (option) {
	case PR_SET_SECCOMP:
#ifdef USE_SECCOMP
//		if (rand() % 3 == SECCOMP_MODE_FILTER) {
			gen_seccomp_bpf((unsigned long *) page_rand, NULL);

			shm->a2[childno] = SECCOMP_MODE_FILTER;
			shm->a3[childno] = (unsigned long) page_rand;
//		}
#endif
		break;
	default:
		break;
	}
}

struct syscall syscall_prctl = {
	.name = "prctl",
	.num_args = 5,
	.arg1name = "option",
	.arg2name = "arg2",
	.arg3name = "arg3",
	.arg4name = "arg4",
	.arg5name = "arg5",
	.sanitise = sanitise_prctl,
};
