#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/syscall.h>   /* For SYS_xxx definitions */
//#include <asm/unistd.h>

#include "perf.h"
#include "shm.h"
#include "log.h"
#include "sanitise.h"

void open_perf_fds(void)
{
	int fd;
	unsigned int i = 0;

	while (i < MAX_PERF_FDS) {
		//sanitise_perf_event_open(0);
		//fd = syscall(__NR_perf_event_open, shm->a1[0], shm->a2[0], shm->a3[0], shm->a4[0], shm->a5[0]);
		if (fd != -1) {
			shm->perf_fds[i] = fd;
			output(2, "fd[%d] = perf\n", shm->perf_fds[i]);
			i++;
		} else {
			if (errno == ENOSYS)
				return;
		}
	}
}
