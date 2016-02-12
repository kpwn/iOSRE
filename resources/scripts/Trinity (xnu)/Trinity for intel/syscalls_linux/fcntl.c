/*
 * SYSCALL_DEFINE3(fcntl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
 *
 * For a successful call, the return value depends on the operation:
 *
 *     F_DUPFD The new descriptor.
 *     F_GETFD Value of file descriptor flags.
 *     F_GETFL Value of file status flags.
 *     F_GETLEASE Type of lease held on file descriptor.
 *     F_GETOWN Value of descriptor owner.
 *     F_GETSIG Value of signal sent when read or write becomes possible, or zero for traditional SIGIO behavior.
 *     F_GETPIPE_SZ The pipe capacity.
 *
 *     All other commands
 *              Zero.
 *
 *     On error, -1 is returned, and errno is set appropriately.
 */

#include <fcntl.h>
#include <signal.h>
#include "random.h"
#include "sanitise.h"
#include "shm.h"
#include "compat.h"

#if F_GETLK64 != F_GETLK
#define HAVE_LK64
#endif

static void sanitise_fcntl(int childno)
{
	switch (shm->a2[childno]) {
	/* arg = fd */
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_SETLEASE:
		shm->a3[childno] = (unsigned long) get_random_fd();
		break;
		break;

	/* no arg */
	case F_GETFD:
	case F_GETFL:
	case F_GETOWN:
	case F_GETSIG:
	case F_GETLEASE:
	case F_GETPIPE_SZ:
		break;

	case F_SETFD:	/* arg = flags */
		shm->a3[childno] = (unsigned int) rand32();
		break;

	case F_SETFL:
		shm->a3[childno] = 0L;
		if (rand_bool())
			shm->a3[childno] |= O_APPEND;
		if (rand_bool())
			shm->a3[childno] |= O_ASYNC;
		if (rand_bool())
			shm->a3[childno] |= O_DIRECT;
		if (rand_bool())
			shm->a3[childno] |= O_NOATIME;
		if (rand_bool())
			shm->a3[childno] |= O_NONBLOCK;
		break;

	/* arg = (struct flock *) */
	case F_GETLK:
	case F_SETLK:
	case F_SETLKW:
		break;
#ifdef HAVE_LK64
	case F_GETLK64:
		break;
	case F_SETLK64:
		break;
	case F_SETLKW64:
		break;
#endif

	case F_SETOWN:
		shm->a3[childno] = (unsigned long) get_pid();
		break;

	/* arg = struct f_owner_ex *) */
	case F_GETOWN_EX:
	case F_SETOWN_EX:
		break;

	case F_SETSIG:
		shm->a3[childno] = (unsigned long) rand32();
		if (shm->a3[childno] == SIGINT)
			shm->a3[childno] = 0; /* restore default (SIGIO) */
		break;

	case F_NOTIFY:
		shm->a3[childno] = 0L;
		if (rand_bool())
			shm->a3[childno] |= DN_ACCESS;
		if (rand_bool())
			shm->a3[childno] |= DN_MODIFY;
		if (rand_bool())
			shm->a3[childno] |= DN_CREATE;
		if (rand_bool())
			shm->a3[childno] |= DN_DELETE;
		if (rand_bool())
			shm->a3[childno] |= DN_RENAME;
		if (rand_bool())
			shm->a3[childno] |= DN_ATTRIB;
		break;

	case F_SETPIPE_SZ:
		shm->a3[childno] = rand32();
		break;

	default:
		break;
	}

}

struct syscall syscall_fcntl = {
	.name = "fcntl",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "cmd",
	.arg2type = ARG_OP,
	.arg2list = {
#ifndef HAVE_LK64
		.num = 20,
#else
		.num = 23,
#endif
		.values = { F_DUPFD, F_DUPFD_CLOEXEC, F_GETFD, F_SETFD, F_GETFL, F_SETFL, F_GETLK, F_SETLK,
		  F_SETLKW, F_GETOWN, F_SETOWN, F_GETOWN_EX, F_SETOWN_EX, F_GETSIG, F_SETSIG, F_GETLEASE,
		  F_SETLEASE, F_NOTIFY, F_SETPIPE_SZ, F_GETPIPE_SZ,
#ifdef HAVE_LK64
		  F_GETLK64, F_SETLK64, F_SETLKW64,
#endif
		},
	},
	.arg3name = "arg",
	.rettype = RET_FD,	//FIXME: Needs to mutate somehow depending on 'cmd'
	.flags = NEED_ALARM,
	.group = GROUP_VFS,
	.sanitise = sanitise_fcntl,
};
