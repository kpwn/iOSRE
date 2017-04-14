/*
 * SYSCALL_DEFINE4(ptrace, long, request, long, pid, long, addr, long, data)
 */
#include <stdlib.h>
#include <linux/ptrace.h>
#include "sanitise.h"
#include "shm.h"
#include "compat.h"
#include "arch.h"


static void sanitise_ptrace(int childno)
{
	unsigned int i;

	/* We must be careful to not give out ARG_PID to ptrace,
	 * because we might end up tracing the parent process, or screen/tmux,
	 * or god knows what else that we don't control, but are allowed to trace.
	 * As we can't send it a CONT, it'll just hang forever.
	 * So we just send ptrace commands to child processes.
	 * The parent will get a WIFSTOPPED, and send a CONT, and the world keeps turning.
	 *
	 * Or at least, that's the theory. In reality, this is currently causing 'no such process' errors.
	 *  but broken is at least better than hanging.
	 */
	i  = rand() % shm->running_childs;
	shm->a2[childno] = shm->pids[i];
}


struct syscall syscall_ptrace = {
	.name = "ptrace",
	.num_args = 4,
	.arg1name = "request",
	.arg1type = ARG_OP,
	.arg1list = {
		.num = 25,
		.values = { PTRACE_TRACEME, PTRACE_PEEKTEXT, PTRACE_PEEKDATA, PTRACE_PEEKUSR,
				PTRACE_POKETEXT, PTRACE_POKEDATA, PTRACE_POKEUSR, PTRACE_GETREGS,
				PTRACE_GETFPREGS, PTRACE_GETSIGINFO, PTRACE_SETREGS, PTRACE_SETFPREGS,
				PTRACE_SETSIGINFO, PTRACE_SETOPTIONS, PTRACE_GETEVENTMSG, PTRACE_CONT,
				PTRACE_SYSCALL, PTRACE_SINGLESTEP, PTRACE_SYSEMU, PTRACE_SYSEMU_SINGLESTEP,
				PTRACE_KILL, PTRACE_ATTACH, PTRACE_DETACH, PTRACE_GETSIGMASK,
				PTRACE_SETSIGMASK },
	},
	.arg2name = "pid",
	.arg3name = "addr",
	.arg3type = ARG_ADDRESS,
	.arg4name = "data",
	.sanitise = sanitise_ptrace,

	.flags = AVOID_SYSCALL,
};
