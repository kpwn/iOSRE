#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include "trinity.h"
#include "child.h"
#include "signals.h"
#include "shm.h"
#include "files.h"
#include "random.h"
#include "syscall.h"
#include "pids.h"
#include "log.h"
#include "params.h"

int check_tainted(void)
{
	int fd;
	unsigned int ret;
	char buffer[11];

	buffer[10] = 0; //make sure that we can fit the whole int.

	fd = open("/proc/sys/kernel/tainted", O_RDONLY);
	if (fd < 0)
		return -1;
	ret = read(fd, buffer, 10);
	close(fd);

	if (ret > 0)
		ret = atoi(buffer);
	else {
		/* We should never fail, but if we do, assume untainted. */
		ret = 0;
	}

	return ret;
}

static void oom_score_adj(int adj)
{
	FILE *fp;

	fp = fopen("/proc/self/oom_score_adj", "w");
	if (!fp)
		return;

	fprintf(fp, "%d", adj);
	fclose(fp);
}

static void fork_children(void)
{
	int pidslot;
	static char childname[17];

	/* Generate children*/

	while (shm->running_childs < shm->max_children) {
		int pid = 0;
		int fd;

		if (shm->spawn_no_more == TRUE)
			return;

		/* a new child means a new seed, or the new child
		 * will do the same syscalls as the one in the pidslot it's replacing.
		 * (special case startup, or we reseed unnecessarily)
		 */
		if (shm->ready == TRUE)
			reseed();

		/* Find a space for it in the pid map */
		pidslot = find_pid_slot(EMPTY_PIDSLOT);
		if (pidslot == PIDSLOT_NOT_FOUND) {
			outputerr("## Pid map was full!\n");
			dump_pid_slots();
			exit(EXIT_FAILURE);
		}

		if (logging == TRUE) {
			fd = fileno(shm->logfiles[pidslot]);
			if (ftruncate(fd, 0) == 0)
				lseek(fd, 0, SEEK_SET);
		}

		(void)alarm(0);
		fflush(stdout);
		pid = fork();
		if (pid != 0)
			shm->pids[pidslot] = pid;
		else {
			/* Child process. */
			int ret = 0;

			mask_signals_child();

			memset(childname, 0, sizeof(childname));
			sprintf(childname, "trinity-child%d", pidslot);
			//prctl(PR_SET_NAME, (unsigned long) &childname);

			oom_score_adj(500);

			/* Wait for parent to set our pidslot */
			while (shm->pids[pidslot] != getpid()) {
				/* Make sure parent is actually alive to wait for us. */
				ret = pid_alive(shm->mainpid);
				if (ret != 0) {
					shm->exit_reason = EXIT_SHM_CORRUPTION;
					outputerr(BUGTXT "parent (%d) went away!\n", shm->mainpid);
					sleep(20000);
				}
			}

			/* Wait for all the children to start up. */
			while (shm->ready == FALSE)
				sleep(1);

			init_child(pidslot);

			ret = child_process(pidslot);

			output(1, "child exiting.\n");

			_exit(ret);
		}
		shm->running_childs++;
		debugf("Created child %d in pidslot %d [total:%d/%d]\n",
			shm->pids[pidslot], pidslot,
			shm->running_childs, shm->max_children);

		if (shm->exit_reason != STILL_RUNNING)
			return;

	}
	shm->ready = TRUE;

	debugf("created enough children\n");
}

void reap_child(pid_t childpid)
{
	int i;

	while (shm->reaper_lock == LOCKED);

	shm->reaper_lock = LOCKED;

	if (childpid == shm->last_reaped) {
		debugf("already reaped %d!\n", childpid);
		goto out;
	}

	i = find_pid_slot(childpid);
	if (i == PIDSLOT_NOT_FOUND)
		goto out;

	debugf("Removing pid %d from pidmap.\n", childpid);
	shm->pids[i] = EMPTY_PIDSLOT;
	shm->running_childs--;
	shm->tv[i].tv_sec = 0;
	shm->last_reaped = childpid;

out:
	shm->reaper_lock = UNLOCKED;
}

static void handle_child(pid_t childpid, int childstatus)
{
	unsigned int i;
	int slot;

	switch (childpid) {
	case 0:
		//debugf("Nothing changed. children:%d\n", shm->running_childs);
		break;

	case -1:
		if (shm->exit_reason != STILL_RUNNING)
			return;

		if (errno == ECHILD) {
			debugf("All children exited!\n");
			for_each_pidslot(i) {
				if (shm->pids[i] != EMPTY_PIDSLOT) {
					if (pid_alive(shm->pids[i]) == -1) {
						debugf("Removing %d from pidmap\n", shm->pids[i]);
						shm->pids[i] = EMPTY_PIDSLOT;
						shm->running_childs--;
					} else {
						debugf("%d looks still alive! ignoring.\n", shm->pids[i]);
					}
				}
			}
			break;
		}
		output(0, "error! (%s)\n", strerror(errno));
		break;

	default:
		debugf("Something happened to pid %d\n", childpid);

		if (WIFEXITED(childstatus)) {

			slot = find_pid_slot(childpid);
			if (slot == PIDSLOT_NOT_FOUND) {
				/* If we reaped it, it wouldn't show up, so check that. */
				if (shm->last_reaped != childpid) {
					outputerr("## Couldn't find pid slot for %d\n", childpid);
					shm->exit_reason = EXIT_LOST_PID_SLOT;
					dump_pid_slots();
				}
			} else {
				debugf("Child %d exited after %ld syscalls.\n", childpid, shm->child_syscall_count[slot]);
				reap_child(childpid);
			}
			break;

		} else if (WIFSIGNALED(childstatus)) {

			switch (WTERMSIG(childstatus)) {
			case SIGALRM:
				debugf("got a alarm signal from pid %d\n", childpid);
				break;
			case SIGFPE:
			case SIGSEGV:
			case SIGKILL:
			case SIGPIPE:
			case SIGABRT:
				debugf("got a signal from pid %d (%s)\n", childpid, strsignal(WTERMSIG(childstatus)));
				reap_child(childpid);
				break;
			default:
				debugf("** Child got an unhandled signal (%d)\n", WTERMSIG(childstatus));
				break;
			}
			break;

		} else if (WIFSTOPPED(childstatus)) {

			switch (WSTOPSIG(childstatus)) {
			case SIGALRM:
				debugf("got an alarm signal from pid %d\n", childpid);
				break;
			case SIGSTOP:
				debugf("Sending PTRACE_DETACH (and then KILL)\n");
				//ptrace(PTRACE_DETACH, childpid, NULL, NULL);
				kill(childpid, SIGKILL);
				reap_child(childpid);
				break;
			case SIGFPE:
			case SIGSEGV:
			case SIGKILL:
			case SIGPIPE:
			case SIGABRT:
				debugf("Child %d was stopped by %s\n", childpid, strsignal(WTERMSIG(childstatus)));
				reap_child(childpid);
				break;
			default:
				debugf("Child %d was stopped by unhandled signal (%s).\n", childpid, strsignal(WSTOPSIG(childstatus)));
				break;
			}
			break;

		} else if (WIFCONTINUED(childstatus)) {
			break;
		} else {
			output(0, "erk, wtf\n");
		}
	}
}

static void handle_children(void)
{
	unsigned int i;
	int childstatus;
	pid_t pid;

	if (shm->running_childs == 0)
		return;

	/* First, we wait for *any* child to wake us up. */
	pid = waitpid(-1, &childstatus, WUNTRACED | WCONTINUED);

	/* We were awoken, handle it. */
	handle_child(pid, childstatus);

	/* While we're awake, let's see if the other children need attention.
	 * We do this instead of just waitpid(-1) again so that there's no way
	 * for any one child to starve the others of attention.
	 */
	for_each_pidslot(i) {

		pid = shm->pids[i];

		if (pid == EMPTY_PIDSLOT)
			continue;

		if (pid_is_valid(pid) == FALSE)
			return;

		pid = waitpid(pid, &childstatus, WUNTRACED | WCONTINUED | WNOHANG);
		if (pid != 0)
			handle_child(pid, childstatus);
	}
}

static const char *reasons[] = {
	"Still running.",
	"No more syscalls enabled.",
	"Reached maximum syscall count.",
	"No file descriptors open.",
	"Lost track of a pid slot.",
	"shm corruption - Found a pid out of range.",
	"ctrl-c",
	"kernel became tainted.",
	"SHM was corrupted!",
	"Child reparenting problem",
	"No files in file list.",
	"Main process disappeared.",
	"UID changed.",
};

static const char * decode_exit(unsigned int reason)
{
	return reasons[reason];
}

static void main_loop(void)
{
	while (shm->exit_reason == STILL_RUNNING) {

		if (shm->spawn_no_more == FALSE) {
			if (shm->running_childs < shm->max_children)
				fork_children();

			/* Periodic regenation of fd's etc. */
			if (shm->regenerate >= REGENERATION_POINT)
				regenerate();

			if (shm->need_reseed == TRUE)
				reseed();
		}

		handle_children();
	}
}


void do_main_loop(void)
{
	const char taskname[13]="trinity-main";
	int childstatus;
	pid_t pid;

	/* do an extra fork so that the watchdog and the children don't share a common parent */
	fflush(stdout);
	pid = fork();
	if (pid == 0) {
		setup_main_signals();

		shm->mainpid = getpid();
		output(0, "Main thread is alive.\n");
		//prctl(PR_SET_NAME, (unsigned long) &taskname);
		set_seed(0);

		setup_fds();
		if (no_files == FALSE) {
			if (files_in_index == 0) {
				shm->exit_reason = EXIT_NO_FILES;
				_exit(EXIT_FAILURE);
			}
		}

		main_loop();

		/* Wait until all children have exited. */
		while (pidmap_empty() == FALSE)
			handle_children();

		outputerr("Bailing main loop. Exit reason: %s\n", decode_exit(shm->exit_reason));
		_exit(EXIT_SUCCESS);
	}

	/* wait for main loop process to exit. */
	pid = waitpid(pid, &childstatus, 0);
}
