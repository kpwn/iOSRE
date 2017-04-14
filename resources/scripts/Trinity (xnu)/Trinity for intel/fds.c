#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/epoll.h>
//#include <sys/eventfd.h>

#include "perf.h"
#include "random.h"
#include "shm.h"
#include "files.h"
#include "pids.h"
#include "net.h"
#include "log.h"
#include "sanitise.h"
#include "params.h"

/* Pipe FD related functions. */
static void open_pipes(void)
{
	int pipes[2];
	unsigned int i;

	for (i = 0; i < MAX_PIPE_FDS; i+=2) {
		if (pipe(pipes) < 0) {
			perror("pipe fail.\n");
			exit(EXIT_FAILURE);
		}
		shm->pipe_fds[i] = pipes[0];
		shm->pipe_fds[i+1] = pipes[1];

		output(2, "fd[%d] = pipe\n", shm->pipe_fds[i]);
		output(2, "fd[%d] = pipe\n", shm->pipe_fds[i+1]);
	}
}

static int rand_pipe_fd(void)
{
	return shm->pipe_fds[rand() % MAX_PIPE_FDS];
}

/* perf related fds (see also perf.c & syscalls/perf_event_open.c) */
static int rand_perf_fd(void)
{
	return shm->perf_fds[rand() % MAX_PERF_FDS];
}

/* epoll related fds */
static void open_epoll_fds(void)
{
	unsigned int i = 0;
	int fd = -1;

	/*while (i < MAX_EPOLL_FDS) {

		switch (rand_bool()) {
		case 0:	fd = epoll_create(1);
			break;
		case 1:	fd = epoll_create1(EPOLL_CLOEXEC);
			break;
		default:
			 break;
		}

		if (fd != -1) {
			shm->epoll_fds[i] = fd;
			output(2, "fd[%d] = epoll\n", shm->epoll_fds[i]);
			i++;
		}
	}*/
}

static int rand_epoll_fd(void)
{
	return shm->epoll_fds[rand() % MAX_EPOLL_FDS];
}

/* eventfd FDs */
static void open_eventfd_fds(void)
{
	unsigned int i;

	/*shm->eventfd_fds[0] = eventfd(rand32(), 0);
	shm->eventfd_fds[1] = eventfd(rand32(), EFD_CLOEXEC);
	shm->eventfd_fds[2] = eventfd(rand32(), EFD_NONBLOCK);
	shm->eventfd_fds[3] = eventfd(rand32(), EFD_SEMAPHORE);
	shm->eventfd_fds[4] = eventfd(rand32(), EFD_CLOEXEC | EFD_NONBLOCK);
	shm->eventfd_fds[5] = eventfd(rand32(), EFD_CLOEXEC | EFD_SEMAPHORE);
	shm->eventfd_fds[6] = eventfd(rand32(), EFD_NONBLOCK | EFD_SEMAPHORE);
	shm->eventfd_fds[7] = eventfd(rand32(), EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE);

	for (i = 0; i < MAX_EVENTFD_FDS; i++)
		output(2, "fd[%d] = eventfd\n", shm->eventfd_fds[i]);*/
}

static int rand_eventfd_fd(void)
{
	return shm->eventfd_fds[rand() % MAX_EVENTFD_FDS];
}

/* regular file FDs  */
unsigned int nr_file_fds = 0;

static int rand_file_fd(void)
{
	unsigned int fd_index;

	fd_index = rand() % nr_file_fds;
	return shm->file_fds[fd_index];
}


static int get_new_random_fd(void)
{
	unsigned int i;
	int fd = 0;

retry:
	i = rand() % 6;

	if (do_specific_proto == TRUE)
		i = 1;

	if (no_files == TRUE)
		i = 1;

	/* Ugly special case.
	 * Sometimes, we can get here without any fd's setup.
	 * If this happens, we divide by zero if we pick case 0 because
	 * nr_file_fds is zero
	 *
	 * When this circumstance occurs, we just force it to use another network socket.
	 *
	 * FIXME: A better solution would be to like, actually open an fd. duh.
	 */
	if (nr_file_fds == 0)
		i = 1;

	switch (i) {
	case 0:
		fd = rand_file_fd();
		break;

	case 1:
		/* When using victim files, sockets can be 0.
		 * Use files as a fallback, or pipes if no files are open.
		 */
		if (nr_sockets == 0) {
			if (nr_file_fds > 0)
				fd = rand_file_fd();
			else
				fd = rand_pipe_fd();
			return fd;
		}
		fd = shm->sockets[rand() % nr_sockets].fd;
		break;

	case 2:
		fd = rand_pipe_fd();
		break;

	case 3:
		if (shm->perf_fds[0] == 0)	/* perf unavailable/disabled. */
			goto retry;

		fd = rand_perf_fd();
		break;

	case 4:
		fd = rand_epoll_fd();
		break;

	case 5:
		fd = rand_eventfd_fd();
		break;

	default:
		break;
	}

	return fd;
}

int get_random_fd(void)
{
	/* 25% chance of returning something new. */
	if ((rand() % 4) == 0)
		return get_new_random_fd();

	/* the rest of the time, return the same fd as last time. */
regen:
	if (shm->fd_lifetime == 0) {
		shm->current_fd = get_new_random_fd();
		shm->fd_lifetime = (rand() % shm->max_children) + 5;
	} else
		shm->fd_lifetime--;

	if (shm->current_fd == 0) {
		shm->fd_lifetime = 0;
		goto regen;
	}

	return shm->current_fd;
}

void setup_fds(void)
{
	/* If we have victim files, don't worry about sockets. */
	/*if (victim_path == NULL)
		open_sockets();*/

	//open_pipes();

	//open_perf_fds();

	//open_epoll_fds();

	//open_eventfd_fds();

	if (no_files == FALSE) {
		generate_filelist();
		if (files_in_index == 0)	/* Something bad happened. Crappy -V maybe? */
			return;			// FIXME: We should log something here probably.

		open_files();
	}
}

void regenerate_fds(void)
{
	if (no_files == TRUE)
		return;

	close_files();
	open_files();
}
