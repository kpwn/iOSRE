#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <setjmp.h>
//#include <malloc.h>
//#include <asm/unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/socket.h>

#include "arch.h"
#include "trinity.h"
#include "files.h"
#include "log.h"
#include "maps.h"
#include "pids.h"
#include "net.h"
#include "params.h"
#include "random.h"
#include "signals.h"
#include "shm.h"
#include "syscall.h"
#include "tables.h"
#include "ioctls.h"
#include "protocols.h"
#include "config.h"	// for VERSION

char *progname = NULL;

unsigned int page_size;
unsigned int num_online_cpus;

struct shm_s *shm;

#define SHM_PROT_PAGES 30

static int create_shm(void)
{
	void *p;
	unsigned int shm_pages;

	shm_pages = ((sizeof(struct shm_s) + page_size - 1) & ~(page_size - 1)) / page_size;

	/* Waste some address space to set up some "protection" near the SHM location. */
	p = alloc_shared((SHM_PROT_PAGES + shm_pages + SHM_PROT_PAGES) * page_size);
	if (p == NULL) {
		perror("mmap");
		return -1;
	}

	mprotect(p, SHM_PROT_PAGES * page_size, PROT_NONE);
	mprotect(p + (SHM_PROT_PAGES + shm_pages) * page_size,
			SHM_PROT_PAGES * page_size, PROT_NONE);

	shm = p + SHM_PROT_PAGES * page_size;

	memset(shm, 0, sizeof(struct shm_s));

	shm->total_syscalls_done = 1;
	shm->regenerate = 0;

	memset(shm->pids, EMPTY_PIDSLOT, sizeof(shm->pids));

	shm->nr_active_syscalls = 0;
	shm->nr_active_32bit_syscalls = 0;
	shm->nr_active_64bit_syscalls = 0;
	memset(shm->active_syscalls, 0, sizeof(shm->active_syscalls));
	memset(shm->active_syscalls32, 0, sizeof(shm->active_syscalls32));
	memset(shm->active_syscalls64, 0, sizeof(shm->active_syscalls64));

	/* Overwritten later in setup_shm_postargs if user passed -s */
	shm->seed = new_seed();

	/* Set seed in parent thread */
	set_seed(0);

	return 0;
}

static void setup_shm_postargs(void)
{
	if (user_set_seed == TRUE) {
		shm->seed = init_seed(seed);
		/* Set seed in parent thread */
		set_seed(0);
	}

	if (user_specified_children != 0)
		shm->max_children = user_specified_children;
	else
		shm->max_children = sysconf(_SC_NPROCESSORS_ONLN);

	if (shm->max_children > MAX_NR_CHILDREN) {
		outputerr("Increase MAX_NR_CHILDREN!\n");
		exit(EXIT_FAILURE);
	}
}

/* This is run *after* we've parsed params */
static int munge_tables(void)
{
	unsigned int ret;

	/* By default, all syscall entries will be disabled.
	 * If we didn't pass -c, -x or -r, mark all syscalls active.
	 */
	if ((do_specific_syscall == FALSE) && (do_exclude_syscall == FALSE) && (random_selection == FALSE) && (desired_group == GROUP_NONE))
		mark_all_syscalls_active();

	if (desired_group != GROUP_NONE) {
		ret = setup_syscall_group(desired_group);
		if (ret == FALSE)
			return FALSE;
	}

	if (random_selection == TRUE)
		enable_random_syscalls();

	/* If we saw a '-x', set all syscalls to enabled, then selectively disable.
	 * Unless we've started enabling them already (with -r)
	 */
	if (do_exclude_syscall == TRUE) {
		if (random_selection == FALSE)
			mark_all_syscalls_active();
		deactivate_disabled_syscalls();
	}

	/* if we passed -n, make sure there's no VM/VFS syscalls enabled. */
	if (no_files == TRUE)
		disable_non_net_syscalls();

	sanity_check_tables();

	count_syscalls_enabled();

	if (verbose == TRUE)
		display_enabled_syscalls();

	if (validate_syscall_tables() == FALSE) {
		outputstd("No syscalls were enabled!\n");
		outputstd("Use 32bit:%d 64bit:%d\n", use_32bit, use_64bit);
		return FALSE;
	}

	return TRUE;
}

/*
 * just in case we're not using the test.sh harness, we
 * change to the tmp dir if it exists.
 */
static void change_tmp_dir(void)
{
	struct stat sb;
	const char tmpdir[]="tmp/";
	int ret;

	/* Check if it exists, bail early if it doesn't */
	ret = (lstat(tmpdir, &sb));
	if (ret == -1)
		return;

	/* Just in case a previous run screwed the perms. */
	ret = chmod(tmpdir, 0755);
	if (ret == -1)
		output(0, "Couldn't chmod %s to 0755.\n", tmpdir);

	ret = chdir(tmpdir);
	if (ret == -1)
		output(0, "Couldn't change to %s\n", tmpdir);
}


int main(int argc, char* argv[])
{
	int ret = EXIT_SUCCESS;
	int childstatus;
	unsigned int i;

	outputstd("Trinity v" __stringify(VERSION) "  Dave Jones <davej@redhat.com>\n");

	progname = argv[0];

	initpid = getpid();

	page_size = getpagesize();
	num_online_cpus = sysconf(_SC_NPROCESSORS_ONLN);

	select_syscall_tables();

	if (create_shm())
		exit(EXIT_FAILURE);

	parse_args(argc, argv);
	outputstd("Done parsing arguments.\n");

	if (kernel_taint_mask != (int)0xFFFFFFFF) {
		outputstd("Custom kernel taint mask has been specified: 0x%08x (%d).\n", kernel_taint_mask, kernel_taint_mask);
	}

	setup_shm_postargs();

	if (logging == TRUE)
		open_logfiles();

	if (munge_tables() == FALSE) {
		ret = EXIT_FAILURE;
		goto out;
	}

	if (show_syscall_list == TRUE) {
		dump_syscall_tables();
		goto out;
	}

	init_syscalls();

	if (show_ioctl_list == TRUE) {
		dump_ioctls();
		goto out;
	}

	if (getuid() == 0) {
		if (dangerous == TRUE) {
			outputstd("DANGER: RUNNING AS ROOT.\n");
			outputstd("Unless you are running in a virtual machine, this could cause serious problems such as overwriting CMOS\n");
			outputstd("or similar which could potentially make this machine unbootable without a firmware reset.\n\n");
			outputstd("ctrl-c now unless you really know what you are doing.\n");
			for (i = 10; i > 0; i--) {
				outputstd("Continuing in %d seconds.\r", i);
				(void)fflush(stdout);
				sleep(1);
			}
		} else {
			outputstd("Don't run as root (or pass --dangerous if you know what you are doing).\n");
			exit(EXIT_FAILURE);
		}
	}

	/*if (do_specific_proto == TRUE)
		find_specific_proto(specific_proto_optarg);*/

	init_buffers();

	parse_devices();

	pids_init();

	setup_main_signals();

	kernel_taint_initial = check_tainted();
	if (kernel_taint_initial != 0) {
		output(0, "Kernel was tainted on startup. Will ignore flags that are already set.\n");
	}

	change_tmp_dir();

	/* check if we ctrl'c or something went wrong during init. */
	if (shm->exit_reason != STILL_RUNNING)
		goto cleanup_fds;

	init_watchdog();

	do_main_loop();

	/* Shutting down. */
	waitpid(watchdog_pid, &childstatus, 0);

	output(0, "\nRan %ld syscalls. Successes: %ld  Failures: %ld\n",
		shm->total_syscalls_done - 1, shm->successes, shm->failures);

	ret = EXIT_SUCCESS;

cleanup_fds:

	close_sockets();

	destroy_maps();

	if (logging == TRUE)
		close_logfiles();

out:

	exit(ret);
}
