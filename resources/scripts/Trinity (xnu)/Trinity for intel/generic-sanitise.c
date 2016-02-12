#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "trinity.h"	// page_size
#include "files.h"
#include "arch.h"
#include "random.h"
#include "sanitise.h"
#include "syscall.h"
#include "net.h"
#include "log.h"
#include "maps.h"
#include "shm.h"
#include "tables.h"

static unsigned int get_cpu(void)
{
	int i;
	i = rand() % 100;

	switch (i) {
	case 0: return -1;
	case 1: return rand() % 4095;
	case 2: return rand() % 15;

	case 3 ... 99:
		return rand() % num_online_cpus;

	default:
		BUG("unreachable!\n");
		break;
	}
	return 0;
}

static unsigned long handle_arg_address(int childno, int call, int argnum)
{
	unsigned long addr = 0;

	if (rand_bool())
		return (unsigned long) get_address();

	/* Half the time, we look to see if earlier args were also ARG_ADDRESS,
	 * and munge that instead of returning a new one from get_address() */

	addr = find_previous_arg_address(argnum, call, childno);

	switch (rand() % 4) {
	case 0:	break;	/* return unmodified */
	case 1:	addr++;
		break;
	case 2:	addr+= sizeof(int);
		break;
	case 3:	addr+= sizeof(long);
		break;
	default: BUG("unreachable!\n");
		break;
	}

	return addr;
}

static unsigned long handle_arg_range(unsigned int call, unsigned int argnum)
{
	unsigned long i;
	unsigned long low = 0, high = 0;

	switch (argnum) {
	case 1:	low = syscalls[call].entry->low1range;
		high = syscalls[call].entry->hi1range;
		break;
	case 2:	low = syscalls[call].entry->low2range;
		high = syscalls[call].entry->hi2range;
		break;
	case 3:	low = syscalls[call].entry->low3range;
		high = syscalls[call].entry->hi3range;
		break;
	case 4:	low = syscalls[call].entry->low4range;
		high = syscalls[call].entry->hi4range;
		break;
	case 5:	low = syscalls[call].entry->low5range;
		high = syscalls[call].entry->hi5range;
		break;
	case 6:	low = syscalls[call].entry->low6range;
		high = syscalls[call].entry->hi6range;
		break;
	default:
		BUG("Should never happen.\n");
		break;
	}

	if (high == 0) {
		outputerr("%s forgets to set hirange!\n", syscalls[call].entry->name);
		BUG("Fix syscall definition!\n");
		return 0;
	}

	i = (unsigned long) rand64() % high;
	if (i < low) {
		i += low;
		i &= high;
	}
	return i;
}

static unsigned long handle_arg_op(unsigned long call, unsigned long argnum)
{
	const unsigned int *values = NULL;
	unsigned int num = 0;
	unsigned long mask = 0;

	switch (argnum) {
	case 1:	num = syscalls[call].entry->arg1list.num;
		values = syscalls[call].entry->arg1list.values;
		break;
	case 2:	num = syscalls[call].entry->arg2list.num;
		values = syscalls[call].entry->arg2list.values;
		break;
	case 3:	num = syscalls[call].entry->arg3list.num;
		values = syscalls[call].entry->arg3list.values;
		break;
	case 4:	num = syscalls[call].entry->arg4list.num;
		values = syscalls[call].entry->arg4list.values;
		break;
	case 5:	num = syscalls[call].entry->arg5list.num;
		values = syscalls[call].entry->arg5list.values;
		break;
	case 6:	num = syscalls[call].entry->arg6list.num;
		values = syscalls[call].entry->arg6list.values;
		break;
	default: break;
	}

	if (num == 0)
		BUG("ARG_OP with 0 args. What?\n");

	if (values == NULL)
		BUG("ARG_OP with no values.\n");

	mask |= values[rand() % num];
	return mask;
}

static unsigned long handle_arg_list(unsigned long call, unsigned long argnum)
{
	unsigned long i;
	unsigned long mask = 0;
	unsigned int bits;
	unsigned int num = 0;
	const unsigned int *values = NULL;

	switch (argnum) {
	case 1:	num = syscalls[call].entry->arg1list.num;
		values = syscalls[call].entry->arg1list.values;
		break;
	case 2:	num = syscalls[call].entry->arg2list.num;
		values = syscalls[call].entry->arg2list.values;
		break;
	case 3:	num = syscalls[call].entry->arg3list.num;
		values = syscalls[call].entry->arg3list.values;
		break;
	case 4:	num = syscalls[call].entry->arg4list.num;
		values = syscalls[call].entry->arg4list.values;
		break;
	case 5:	num = syscalls[call].entry->arg5list.num;
		values = syscalls[call].entry->arg5list.values;
		break;
	case 6:	num = syscalls[call].entry->arg6list.num;
		values = syscalls[call].entry->arg6list.values;
		break;
	default: break;
	}

	if (num == 0)
		BUG("ARG_LIST with 0 args. What?\n");

	if (values == NULL)
		BUG("ARG_LIST with no values.\n");

	bits = rand() % (num + 1);	/* num of bits to OR */
	for (i = 0; i < bits; i++)
		mask |= values[rand() % num];
	return mask;
}

static unsigned long handle_arg_randpage(void)
{
	/* Because we pass this to something that might overwrite it,
	 * and we want page_allocs to remain unmodified, we copy it to page rand instead.
	 */
	if (rand_bool())
		memcpy(page_rand, page_allocs, page_size);

	return (unsigned long) page_rand;
}

static unsigned long handle_arg_iovec(int childno, unsigned long call, unsigned long argnum)
{
	unsigned long i;

	i = (rand() % 5) + 1;

	switch (argnum) {
	case 1:	if (syscalls[call].entry->arg2type == ARG_IOVECLEN)
			shm->a2[childno] = i;
		break;
	case 2:	if (syscalls[call].entry->arg3type == ARG_IOVECLEN)
			shm->a3[childno] = i;
		break;
	case 3:	if (syscalls[call].entry->arg4type == ARG_IOVECLEN)
			shm->a4[childno] = i;
		break;
	case 4:	if (syscalls[call].entry->arg5type == ARG_IOVECLEN)
			shm->a5[childno] = i;
		break;
	case 5:	if (syscalls[call].entry->arg6type == ARG_IOVECLEN)
			shm->a6[childno] = i;
		break;
	default: BUG("impossible\n");
	}
	return (unsigned long) alloc_iovec(i);
}

static unsigned long handle_arg_len_already_set(int childno, unsigned long argnum)
{
	unsigned long r = 0;

	/*
	 * We already set the len in the ARG_IOVEC/ARG_SOCKADDR case
	 * So here we just return what we had set there.
	 */
	switch (argnum) {
	case 1:	r = shm->a1[childno]; break;
	case 2:	r = shm->a2[childno]; break;
	case 3:	r = shm->a3[childno]; break;
	case 4:	r = shm->a4[childno]; break;
	case 5:	r = shm->a5[childno]; break;
	case 6:	r = shm->a6[childno]; break;
	default: break;
	}
	return r;
}

static unsigned long handle_arg_sockaddr(int childno, unsigned long call, unsigned long argnum)
{
	unsigned long sockaddr = 0, sockaddrlen = 0;

	//generate_sockaddr(&sockaddr, &sockaddrlen, PF_NOHINT);

	switch (argnum) {
	case 1:	if (syscalls[call].entry->arg2type == ARG_SOCKADDRLEN)
			shm->a2[childno] = sockaddrlen;
		break;
	case 2:	if (syscalls[call].entry->arg3type == ARG_SOCKADDRLEN)
			shm->a3[childno] = sockaddrlen;
		break;
	case 3:	if (syscalls[call].entry->arg4type == ARG_SOCKADDRLEN)
			shm->a4[childno] = sockaddrlen;
		break;
	case 4:	if (syscalls[call].entry->arg5type == ARG_SOCKADDRLEN)
			shm->a5[childno] = sockaddrlen;
		break;
	case 5:	if (syscalls[call].entry->arg6type == ARG_SOCKADDRLEN)
			shm->a6[childno] = sockaddrlen;
		break;
	case 6:
	default: BUG("impossible\n");
	}
	return (unsigned long) sockaddr;
}

static unsigned long handle_arg_mode_t(void)
{
	unsigned int i, j, count, bit;
	mode_t mode = 0;

	count = rand() % 9;

	for (i = 0; i < count; i++) {
		bit = rand() % 3;
		mode |= 1 << bit;
		j = rand() % 12;
		switch (j) {
		case 0: mode |= S_IRUSR; break;
		case 1: mode |= S_IWUSR; break;
		case 2: mode |= S_IXUSR; break;
		case 3: mode |= S_IRGRP; break;
		case 4: mode |= S_IWGRP; break;
		case 5: mode |= S_IXGRP; break;
		case 6: mode |= S_IROTH; break;
		case 7: mode |= S_IWOTH; break;
		case 8: mode |= S_IXOTH; break;
		case 9: mode |= S_ISUID; break;
		case 10: mode|= S_ISGID; break;
		case 11: mode|= S_ISVTX; break;
		default: break;
		}
	}
	return mode;
}


static unsigned long fill_arg(int childno, int call, unsigned int argnum)
{
	enum argtype argtype = 0;

	if (argnum > syscalls[call].entry->num_args)
		return 0;

	switch (argnum) {
	case 1:	argtype = syscalls[call].entry->arg1type;
		break;
	case 2:	argtype = syscalls[call].entry->arg2type;
		break;
	case 3:	argtype = syscalls[call].entry->arg3type;
		break;
	case 4:	argtype = syscalls[call].entry->arg4type;
		break;
	case 5:	argtype = syscalls[call].entry->arg5type;
		break;
	case 6:	argtype = syscalls[call].entry->arg6type;
		break;
	default:
		BUG("unreachable!\n");
		return 0;
	}

	switch (argtype) {
	case ARG_UNDEFINED:
	case ARG_RANDOM_LONG:
		return (unsigned long) rand64();

	case ARG_FD:
		return get_random_fd();

	case ARG_LEN:
		return (unsigned long) get_len();

	case ARG_ADDRESS:
		return handle_arg_address(childno, call, argnum);

	case ARG_NON_NULL_ADDRESS:
		return (unsigned long) get_non_null_address();

	case ARG_PID:
		return (unsigned long) get_pid();

	case ARG_RANGE:
		return handle_arg_range(call, argnum);

	case ARG_OP:	/* Like ARG_LIST, but just a single value. */
		return handle_arg_op(call, argnum);

	case ARG_LIST:
		return handle_arg_list(call, argnum);

	case ARG_RANDPAGE:
		return handle_arg_randpage();

	case ARG_CPU:
		return (unsigned long) get_cpu();

	case ARG_PATHNAME:
		return (unsigned long) generate_pathname();

	case ARG_IOVEC:
		return handle_arg_iovec(childno, call, argnum);

	case ARG_IOVECLEN:
	case ARG_SOCKADDRLEN:
		return handle_arg_len_already_set(childno, argnum);

	case ARG_SOCKADDR:
		return handle_arg_sockaddr(childno, call, argnum);

	case ARG_MODE_T:
		return handle_arg_mode_t();

	default:
		BUG("unreachable!\n");
		return 0;
	}

	BUG("unreachable!\n");
	return 0x5a5a5a5a;	/* Should never happen */
}

void generic_sanitise(int childno)
{
	unsigned int call = shm->syscallno[childno];

	if (syscalls[call].entry->arg1type != 0)
		shm->a1[childno] = fill_arg(childno, call, 1);
	if (syscalls[call].entry->arg2type != 0)
		shm->a2[childno] = fill_arg(childno, call, 2);
	if (syscalls[call].entry->arg3type != 0)
		shm->a3[childno] = fill_arg(childno, call, 3);
	if (syscalls[call].entry->arg4type != 0)
		shm->a4[childno] = fill_arg(childno, call, 4);
	if (syscalls[call].entry->arg5type != 0)
		shm->a5[childno] = fill_arg(childno, call, 5);
	if (syscalls[call].entry->arg6type != 0)
		shm->a6[childno] = fill_arg(childno, call, 6);
}
