/*
 * Functions for actually doing the system calls.
 */

#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#include "arch.h"
#include "child.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"
#include "syscall.h"
#include "pids.h"
#include "log.h"
#include "params.h"
#include "maps.h"
#include "trinity.h"
#include "tables.h"

#define __syscall_return(type, res) \
	do { \
	if ((unsigned long)(res) >= (unsigned long)(-125)) { \
		errno = -(res); \
		res = -1; \
	} \
	return (type) (res); \
} while (0)

#ifdef ARCH_IS_BIARCH
/*
 * This routine does 32 bit syscalls on 64 bit kernel.
 * 32-on-32 will just use syscall() directly from do_syscall() because shm->do32bit is biarch only.
 */
long syscall32(unsigned int call,
	unsigned long a1, unsigned long a2, unsigned long a3,
	unsigned long a4, unsigned long a5, unsigned long a6)
{
	long __res = 0;

//FIXME: Move the implementations out to arch header files.

#if defined(__x86_64__)
	__asm__ volatile (
		"pushq %%rbp\n\t"
		"pushq %%r10\n\t"
		"pushq %%r11\n\t"
		"movq %7, %%rbp\n\t"
		"int $0x80\n\t"
		"popq %%r11\n\t"
		"popq %%r10\n\t"
		"popq %%rbp\n\t"
		: "=a" (__res)
		: "0" (call),"b" ((long)(a1)),"c" ((long)(a2)),"d" ((long)(a3)), "S" ((long)(a4)),"D" ((long)(a5)), "g" ((long)(a6))
		: "%rbp" /* mark EBP reg as dirty */
	);
	__syscall_return(long, __res);

#else
	/* non-x86 implementations go here. */
	#error Implement 32-on-64 syscall in syscall.c:syscall32() for this architecture.

#endif
	return __res;
}
#else
#define syscall32(a,b,c,d,e,f,g) 0
#endif /* ARCH_IS_BIARCH */

static void check_uid(uid_t olduid)
{
	uid_t myuid;

	myuid = getuid();
	if (myuid != olduid) {

		/* unshare() can change us to /proc/sys/kernel/overflowuid */
		if (myuid == 65534)
			return;

		output(0, "uid changed! Was: %d, now %d\n", olduid, myuid);

		shm->exit_reason = EXIT_UID_CHANGED;
		_exit(EXIT_FAILURE);
	}
}

static unsigned long do_syscall(int childno, int *errno_saved)
{
	int nr = shm->syscallno[childno];
	unsigned long a1, a2, a3, a4, a5, a6;
	unsigned long ret = 0;
	int pidslot;

	a1 = shm->a1[childno];
	a2 = shm->a2[childno];
	a3 = shm->a3[childno];
	a4 = shm->a4[childno];
	a5 = shm->a5[childno];
	a6 = shm->a6[childno];

	pidslot = find_pid_slot(getpid());
	if (pidslot != PIDSLOT_NOT_FOUND) {
		shm->total_syscalls_done++;
		shm->child_syscall_count[pidslot]++;
		(void)gettimeofday(&shm->tv[pidslot], NULL);
	}

	if (syscalls[nr].entry->flags & NEED_ALARM)
		(void)alarm(1);

	errno = 0;

	if (shm->do32bit[childno] == FALSE) {
    //printf("calling syscall %d\n", nr);
		ret = syscall(nr, a1, a2, a3, a4, a5, a6);
	} else
		ret = syscall32(nr, a1, a2, a3, a4, a5, a6);

	*errno_saved = errno;

	if (syscalls[nr].entry->flags & NEED_ALARM)
		(void)alarm(0);

	return ret;
}

/*
 * Generate arguments, print them out, then call the syscall.
 */
long mkcall(int childno)
{
	unsigned int call = shm->syscallno[childno];
	unsigned long ret = 0;
	int errno_saved;
	uid_t olduid = getuid();

	shm->regenerate++;

	shm->a1[childno] = (unsigned long)rand64();
	shm->a2[childno] = (unsigned long)rand64();
	shm->a3[childno] = (unsigned long)rand64();
	shm->a4[childno] = (unsigned long)rand64();
	shm->a5[childno] = (unsigned long)rand64();
	shm->a6[childno] = (unsigned long)rand64();

	generic_sanitise(childno);
	if (syscalls[call].entry->sanitise)
		syscalls[call].entry->sanitise(childno);

	output_syscall_prefix(childno, call);

	/* If we're going to pause, might as well sync pre-syscall */
	if (dopause == TRUE)
		synclogs();

	if (((unsigned long)shm->a1 == (unsigned long) shm) ||
	    ((unsigned long)shm->a2 == (unsigned long) shm) ||
	    ((unsigned long)shm->a3 == (unsigned long) shm) ||
	    ((unsigned long)shm->a4 == (unsigned long) shm) ||
	    ((unsigned long)shm->a5 == (unsigned long) shm) ||
	    ((unsigned long)shm->a6 == (unsigned long) shm)) {
		BUG("Address of shm ended up in a register!\n");
	}

	/* Some architectures (IA64/MIPS) start their Linux syscalls
	 * At non-zero, and have other ABIs below.
	 */
	call += SYSCALL_OFFSET;

	ret = do_syscall(childno, &errno_saved);

	if (IS_ERR(ret))
		shm->failures++;
	else
		shm->successes++;

	output_syscall_postfix(ret, errno_saved, IS_ERR(ret));
	if (dopause == TRUE)
		sleep(1);

	/* If the syscall doesn't exist don't bother calling it next time. */
	if ((ret == -1UL) && (errno_saved == ENOSYS)) {

		/* Futex is awesome, it ENOSYS's depending on arguments. Sigh. */
		if (call == (unsigned int) search_syscall_table(syscalls, max_nr_syscalls, "futex"))
			goto skip_enosys;

		/* Unknown ioctls also ENOSYS. */
		if (call == (unsigned int) search_syscall_table(syscalls, max_nr_syscalls, "ioctl"))
			goto skip_enosys;

		/* sendfile() may ENOSYS depending on args. */
		if (call == (unsigned int) search_syscall_table(syscalls, max_nr_syscalls, "sendfile"))
			goto skip_enosys;

		output(1, "%s (%d) returned ENOSYS, marking as inactive.\n",
			syscalls[call].entry->name, call);

		if (biarch == FALSE) {
			deactivate_syscall(call);
		} else {
			if (shm->do32bit[childno] == TRUE)
				deactivate_syscall32(call);
			else
				deactivate_syscall64(call);
		}
	}

skip_enosys:

	if (syscalls[call].entry->post)
	    syscalls[call].entry->post(ret);

	/* store info for debugging. */
	shm->previous_syscallno[childno] = shm->syscallno[childno];
	shm->previous_a1[childno] = shm->a1[childno];
	shm->previous_a2[childno] = shm->a2[childno];
	shm->previous_a3[childno] = shm->a3[childno];
	shm->previous_a4[childno] = shm->a4[childno];
	shm->previous_a5[childno] = shm->a5[childno];
	shm->previous_a6[childno] = shm->a6[childno];

	check_uid(olduid);

	return ret;
}
