#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include "shm.h"
#include "params.h"	// for 'dangerous'
#include "pids.h"
#include "log.h"
#include "sanitise.h"

pid_t initpid;

int find_pid_slot(pid_t mypid)
{
	unsigned int i;

	for_each_pidslot(i) {
		if (shm->pids[i] == mypid)
			return i;
	}
	return PIDSLOT_NOT_FOUND;
}

bool pidmap_empty(void)
{
	unsigned int i;

	for_each_pidslot(i) {
		if (shm->pids[i] != EMPTY_PIDSLOT)
			return FALSE;
	}
	return TRUE;
}

void dump_pid_slots(void)
{
	unsigned int i, j = 0;
	char string[512], *sptr = string;

	sptr += sprintf(sptr, "## pids: (%d active)\n", shm->running_childs);

	for (i = 0; i < shm->max_children; i+=8) {
		sptr += sprintf(sptr, "%d-%d: ", i, i+7);
		for (j = 0; j < 8; j++) {
			if (shm->pids[i+j] != EMPTY_PIDSLOT) {
				if (pid_alive(shm->pids[i+j] == -1))
					RED
			}

			sptr += sprintf(sptr, "%d ", shm->pids[i+j]);
			CRESET
		}
		sptr += sprintf(sptr, "\n");
	}
	*sptr = '\0';
	outputerr("%s", string);
}

static pid_t pidmax;

static int read_pid_max(void)
{
	unsigned long result;
	char *end, buf[32];
	FILE *fp;
	int rc;

	fp = fopen("/proc/sys/kernel/pid_max", "r");
	if (!fp) {
		perror("fopen");
		return -1;
	}

	rc = -1;
	if (!fgets(buf, sizeof(buf), fp))
		goto out;

	result = strtoul(buf, &end, 10);
	if (end == buf)
		goto out;

	pidmax = result;
	rc = 0;
out:
	fclose(fp);
	return rc;
}

void pids_init(void)
{
	if (read_pid_max()) {
#ifdef __x86_64__
		pidmax = 4194304;
#else
		pidmax = 32768;
#endif
		outputerr("Couldn't read pid_max from proc\n");
	}

	output(0, "Using pid_max = %d\n", pidmax);
}

int pid_is_valid(pid_t pid)
{
	if ((pid > pidmax) || (pid < 1)) {
		output(0, "Sanity check failed! Found pid %d!\n", pid);
		return FALSE;
	}

	return TRUE;
}

unsigned int get_pid(void)
{
	unsigned int i;
	pid_t pid = 0;

	/* If we get called from the parent, and there are no
	 * children around yet, we need to not look at the pidmap. */
	if (shm->running_childs == 0)
		return 0;

	switch (rand() % 3) {
	case 0:
retry:		i = rand() % shm->max_children;
		pid = shm->pids[i];
		if (pid == EMPTY_PIDSLOT)
			goto retry;
		break;

	case 1:	pid = 0;
		break;

	case 2:	if (dangerous == FALSE)	// We don't want root trying to kill init.
			pid = 1;
		break;

	default:
		break;
	}

	return pid;
}
