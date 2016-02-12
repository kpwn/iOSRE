#include <unistd.h>
#include "files.h"
#include "log.h"
#include "maps.h"
#include "net.h"
#include "params.h"
#include "random.h"
#include "shm.h"
#include "trinity.h"

static void do_sso_sockets(void)
{
	struct sockopt so = { 0, 0, 0, 0 };
	unsigned int i;
	int fd, ret;

	for (i = 0; i < nr_sockets; i++) {
		/* skip over bluetooth due to weird linger bug */
		/*if (shm->sockets[i].triplet.family == PF_BLUETOOTH)
			continue;*/

		fd = shm->sockets[i].fd;
		//do_setsockopt(&so);
		ret = setsockopt(fd, so.level, so.optname, (void *)so.optval, so.optlen);
		if (ret == 0)
			output(1, "Setsockopt(%lx %lx %lx %lx) on fd %d [%d:%d:%d]\n",
				so.level, so.optname, so.optval, so.optlen, fd,
				shm->sockets[i].triplet.family,
				shm->sockets[i].triplet.type,
				shm->sockets[i].triplet.protocol);
//		else
//			output(1, "sso failed %s\n", strerror(errno));
	}
}

void regenerate(void)
{
	if (no_files == TRUE)	/* We don't regenerate sockets */
		return;

	/* we're about to exit. */
	if (shm->spawn_no_more)
		return;

	shm->regenerating = TRUE;

	sleep(1);	/* give children time to finish with fds. */

	shm->regenerate = 0;

	output(0, "Regenerating random pages, fd's etc.\n");

	regenerate_fds();

	/* Do random setsockopts on all network sockets. */
	do_sso_sockets();

	destroy_maps();
	setup_maps();

	generate_random_page(page_rand);

	shm->regenerating = FALSE;
}
