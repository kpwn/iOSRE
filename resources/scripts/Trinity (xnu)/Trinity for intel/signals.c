#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "trinity.h"	// __unused__
#include "params.h"	// debug
#include "pids.h"
#include "signals.h"
#include "shm.h"

jmp_buf ret_jump;

int sigwas;

static void ctrlc_handler(__unused__ int sig)
{
	shm->exit_reason = EXIT_SIGINT;
}

static void sighandler(int sig)
{
	int slot;

	sigwas = sig;

	switch (sig) {
	case SIGALRM:
		slot = find_pid_slot(getpid());
		if (slot == PIDSLOT_NOT_FOUND)
			_exit(EXIT_SUCCESS);	/* Hell knows what happened, just bail. */

		/* Check if we're blocking because we're stuck on an fd. */
		if (check_if_fd(slot) == TRUE) {

			/* avoid doing it again from other threads. */
			shm->fd_lifetime = 0;

			/* TODO: Somehow mark the fd in the parent not to be used again too. */
		}

		/* Re-arm the alarm. */
		alarm(1);

		/* TODO: If we get back here after the 10s alarm, we should exit instead of longjmp */

		/* Jump back, maybe we'll make progress. */
		(void)signal(sig, sighandler);
		siglongjmp(ret_jump, 1);
		break;

	default:
		_exit(EXIT_SUCCESS);
	}
}

void mask_signals_child(void)
{
	struct sigaction sa;
	sigset_t ss;
	int i;

	for (i = 1; i < 512; i++) {
		(void)sigfillset(&ss);
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = sighandler;
		sa.sa_mask = ss;
		(void)sigaction(i, &sa, NULL);
	}
	/* we want default behaviour for child process signals */
	(void)signal(SIGCHLD, SIG_DFL);

	/* ignore signals we don't care about */
	(void)signal(SIGFPE, SIG_IGN);
	(void)signal(SIGXCPU, SIG_IGN);
	(void)signal(SIGTSTP, SIG_IGN);
	(void)signal(SIGWINCH, SIG_IGN);
	(void)signal(SIGIO, SIG_IGN);
	(void)signal(SIGPIPE, SIG_IGN);

	/* Ignore the RT signals. */
	/*for (i = SIGRTMIN; i <= SIGRTMAX; i++)
		(void)signal(i, SIG_IGN);*/

	/* If we are in debug mode, we want segfaults and core dumps */
	if (debug == TRUE)
		(void)signal(SIGSEGV, SIG_DFL);

	/* trap ctrl-c */
	(void)signal(SIGINT, ctrlc_handler);
}


void setup_main_signals(void)
{
	/* we want default behaviour for child process signals */
	(void)signal(SIGFPE, SIG_DFL);
	(void)signal(SIGCHLD, SIG_DFL);

	(void)signal(SIGINT, ctrlc_handler);
}
