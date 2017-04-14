/*
 * Each process that gets forked runs this code.
 */

#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
//#include <sys/prctl.h>

#include "child.h"
#include "syscall.h"
#include "log.h"
#include "random.h"
#include "shm.h"
#include "signals.h"
#include "pids.h"
#include "params.h"	// for 'debug'
#include "tables.h"
#include "trinity.h"	// ARRAY_SIZE

static struct rlimit oldrlimit;

static void disable_coredumps(void)
{
	struct rlimit limit;

	if (debug == TRUE) {
		(void)signal(SIGSEGV, SIG_DFL);
		return;
	}

	getrlimit(RLIMIT_CORE, &oldrlimit);

	limit.rlim_cur = 0;
	limit.rlim_max = oldrlimit.rlim_max;
	if (setrlimit(RLIMIT_CORE, &limit) != 0)
		perror( "setrlimit(RLIMIT_CORE)" );
}

static void reenable_coredumps(void)
{
	if (debug == TRUE)
		return;

	//prctl(PR_SET_DUMPABLE, TRUE);

	if (setrlimit(RLIMIT_CORE, &oldrlimit) != 0) {
		outputerr("[%d] Error restoring rlimits to cur:%d max:%d (%s)\n",
			getpid(),
			(unsigned int) oldrlimit.rlim_cur,
			(unsigned int) oldrlimit.rlim_max,
			strerror(errno));
	}
}

static void set_make_it_fail(void)
{
	int fd;
	const char *buf = "1";

	/* If we failed last time, don't bother trying in future. */
	if (shm->do_make_it_fail == TRUE)
		return;

	fd = open("/proc/self/make-it-fail", O_WRONLY);
	if (fd == -1)
		return;

	if (write(fd, buf, 1) == -1) {
		if (errno != EPERM)
			outputerr("writing to /proc/self/make-it-fail failed! (%s)\n", strerror(errno));
		else
			shm->do_make_it_fail = TRUE;
	}
	close(fd);
}

/*
 * We call this occasionally to set some FPU state, in the hopes that we
 * might tickle some weird FPU/scheduler related bugs
 */
static void use_fpu(void)
{
	double x = 0;
	asm volatile("":"+m" (x));
	x += 1;
	asm volatile("":"+m" (x));
}

void init_child(int childno)
{
	//cpu_set_t set;
	pid_t pid = getpid();

	set_seed(childno);

	shm->kill_count[childno] = 0;

	disable_coredumps();

	/*if (sched_getaffinity(pid, sizeof(set), &set) == 0) {
		CPU_ZERO(&set);
		CPU_SET(childno, &set);
		sched_setaffinity(pid, sizeof(set), &set);
	}*/

	shm->child_syscall_count[childno] = 0;

	set_make_it_fail();

	if (rand() % 100 < 50)
		use_fpu();
}

void check_parent_pid(void)
{
	pid_t pid;
	unsigned int i;
	static unsigned int parent_check_time = 10;

	parent_check_time--;
	if (parent_check_time != 0)
		return;

	parent_check_time = 10;

	if (getppid() == shm->mainpid)
		return;

	pid = getpid();

	//FIXME: Add locking so only one child does this output.
	output(0, BUGTXT "CHILD (pid:%d) GOT REPARENTED! "
		"parent pid:%d. Watchdog pid:%d\n",
		pid, shm->mainpid, watchdog_pid);
	output(0, BUGTXT "Last syscalls:\n");

	for (i = 0; i < MAX_NR_CHILDREN; i++) {
		// Skip over 'boring' entries.
		if ((shm->pids[i] == -1) &&
		    (shm->previous_syscallno[i] == 0) &&
		    (shm->child_syscall_count[i] == 0))
			continue;

		output(0, "[%d]  pid:%d call:%s callno:%d\n",
			i, shm->pids[i],
			print_syscall_name(shm->previous_syscallno[i], shm->do32bit[i]),	// FIXME: need previous do32bit
			shm->child_syscall_count[i]);
	}
	shm->exit_reason = EXIT_REPARENT_PROBLEM;
	exit(EXIT_FAILURE);
	//TODO: Emergency logging.
}

struct child_funcs {
	int type;
	const char *name;
	int (*func)(int childno);
};

static const struct child_funcs child_functions[] = {
	{ .type = CHILD_RANDOM_SYSCALLS, .name = "rand_syscalls", .func = child_random_syscalls },
#ifdef DEBUG_MULTI
	{ .type = CHILD_OPEN_ALL_FILES, .name = "read_all_files", .func = child_read_all_files },
#endif
};

int child_process(int childno)
{
	int ret;
	unsigned int i;

	i = rand() % ARRAY_SIZE(child_functions);

#ifdef DEBUG_MULTI
	output(0, "Chose %s.\n", child_functions[i].name);
#endif

	shm->child_type[childno] = child_functions[i].type;
	ret = child_functions[i].func(childno);

	reenable_coredumps();

	return ret;
}
