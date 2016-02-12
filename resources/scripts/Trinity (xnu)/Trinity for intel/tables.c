/*
 * Functions for handling the system call tables.
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "trinity.h"	// ARRAY_SIZE, alloc_shared
#include "arch.h"
#include "arch-syscalls.h"
#include "syscall.h"
#include "params.h"
#include "log.h"
#include "shm.h"
#include "tables.h"

unsigned long syscalls_todo = 0;

bool biarch = FALSE;

int search_syscall_table(const struct syscalltable *table, unsigned int nr_syscalls, const char *arg)
{
	unsigned int i;

	/* search by name */
	for (i = 0; i < nr_syscalls; i++) {
		if (strcmp(arg, table[i].entry->name) == 0) {
			//debugf("Found %s at %u\n", table[i].entry->name, i);
			return i;
		}
	}

	return -1;
}

void validate_specific_syscall(const struct syscalltable *table, int call)
{
	if (call == -1)
		return;

	if (table[call].entry->flags & AVOID_SYSCALL)
		output(0, "%s is marked as AVOID. Skipping\n", table[call].entry->name);

	if (table[call].entry->flags & NI_SYSCALL)
		output(0, "%s is NI_SYSCALL. Skipping\n", table[call].entry->name);
}

int validate_specific_syscall_silent(const struct syscalltable *table, int call)
{
	if (call == -1)
		return FALSE;

	if (table[call].entry->flags & AVOID_SYSCALL)
		return FALSE;

	if (table[call].entry->flags & NI_SYSCALL)
		return FALSE;

	return TRUE;
}

void activate_syscall_in_table(unsigned int calln, unsigned int *nr_active, const struct syscalltable *table, int *active_syscall)
{
	struct syscall *call_ptr;

	call_ptr = table[calln].entry;

	//Check if the call is activated already, and activate it only if needed
	if (call_ptr->active_number == 0) {
		//Sanity check
		if ((*nr_active + 1) > MAX_NR_SYSCALL) {
			output(0, "[tables] MAX_NR_SYSCALL needs to be increased. More syscalls than active table can fit.\n");
			exit(EXIT_FAILURE);
		}

		//save the call no
		active_syscall[*nr_active] = calln + 1;
		(*nr_active) += 1;
		call_ptr->active_number = *nr_active;
	}
}

void deactivate_syscall_in_table(unsigned int calln, unsigned int *nr_active, const struct syscalltable *table, int *active_syscall)
{
	struct syscall *call_ptr;
	unsigned int i;

	call_ptr = table[calln].entry;
	//Check if the call is activated already, and deactivate it only if needed
	if ((call_ptr->active_number != 0) && (*nr_active > 0)) {
		for (i = call_ptr->active_number - 1; i < *nr_active - 1; i++) {
			active_syscall[i] = active_syscall[i + 1];
			table[active_syscall[i] - 1].entry->active_number = i + 1;
		}
		//The last step is to erase the last item.
		active_syscall[*nr_active - 1] = 0;
		(*nr_active) -= 1;
		call_ptr->active_number = 0;
	}
}

void count_syscalls_enabled(void)
{
	if (biarch == TRUE) {
		output(0, "32-bit syscalls: %d enabled, %d disabled.  "
			"64-bit syscalls: %d enabled, %d disabled.\n",
			shm->nr_active_32bit_syscalls, max_nr_32bit_syscalls - shm->nr_active_32bit_syscalls,
			shm->nr_active_64bit_syscalls, max_nr_64bit_syscalls - shm->nr_active_64bit_syscalls);
	} else {
		output(0, "Enabled %d syscalls. Disabled %d syscalls.\n",
			shm->nr_active_syscalls, max_nr_syscalls - shm->nr_active_syscalls);
	}
}

void init_syscalls(void)
{
	if (biarch == TRUE)
		init_syscalls_biarch();
	else
		init_syscalls_uniarch();
}

bool no_syscalls_enabled(void)
{
	if (biarch == TRUE) {
		if ((shm->nr_active_32bit_syscalls == 0) && (shm->nr_active_64bit_syscalls == 0))
			return TRUE;
		else
			return FALSE;
	}

	/* non-biarch */
	if (shm->nr_active_syscalls == 0)
		return TRUE;
	else
		return FALSE;
}

/* Make sure there's at least one syscall enabled. */
int validate_syscall_tables(void)
{
	unsigned int ret;

	if (biarch == TRUE) {
		ret = validate_syscall_table_32();
		ret |= validate_syscall_table_64();
		return ret;
	}

	/* non-biarch case*/
	if (shm->nr_active_syscalls == 0)
		return FALSE;
	else
		return TRUE;
}

static void check_syscall(struct syscall *entry)
{
	/* check that we have a name set. */
#define CHECK(NUMARGS, ARGNUM, ARGTYPE, ARGNAME)		\
	if (entry->num_args > 0) {				\
		if (entry->num_args > NUMARGS) {		\
			if (entry->ARGNAME == NULL)  {		\
				outputerr("arg %d of %s has no name\n", ARGNUM, entry->name);      \
				exit(EXIT_FAILURE);		\
			}					\
		}						\
	}							\

	CHECK(0, 1, arg1type, arg1name);
	CHECK(1, 2, arg2type, arg2name);
	CHECK(2, 3, arg3type, arg3name);
	CHECK(3, 4, arg4type, arg4name);
	CHECK(4, 5, arg5type, arg5name);
	CHECK(5, 6, arg6type, arg6name);

	/* check if we have a type. */
	/* note: not enabled by default, because we haven't annotated everything yet. */
#undef CHECK
#define CHECK(NUMARGS, ARGNUM, ARGTYPE, ARGNAME)		\
	if (entry->num_args > 0) {				\
		if (entry->num_args > NUMARGS) {		\
			if (entry->ARGTYPE == ARG_UNDEFINED) {	\
				outputerr("%s has an undefined argument type for arg1 (%s)!\n", entry->name, entry->ARGNAME);	\
			}					\
		}						\
	}							\

/*	CHECK(0, 1, arg1type, arg1name);
	CHECK(1, 2, arg2type, arg2name);
	CHECK(2, 3, arg3type, arg3name);
	CHECK(3, 4, arg4type, arg4name);
	CHECK(4, 5, arg5type, arg5name);
	CHECK(5, 6, arg6type, arg6name);
*/
}

static void sanity_check(const struct syscalltable *table, unsigned int nr)
{
	unsigned int i;

	for (i = 0; i < nr; i++)
		check_syscall(table[i].entry);
}

void sanity_check_tables(void)
{
	if (biarch == TRUE) {
		sanity_check(syscalls_32bit, max_nr_32bit_syscalls);
		sanity_check(syscalls_64bit, max_nr_64bit_syscalls);
		return;
	}

	/* non-biarch case*/
	sanity_check(syscalls, max_nr_syscalls);
}

void mark_all_syscalls_active(void)
{
	outputstd("Marking all syscalls as enabled.\n");

	if (biarch == TRUE)
		mark_all_syscalls_active_biarch();
	else
		mark_all_syscalls_active_uniarch();
}

void check_user_specified_arch(const char *arg, char **arg_name, bool *only_64bit, bool *only_32bit)
{
	//Check if the arch is specified
	char *arg_arch = strstr(arg,",");
	unsigned long size = 0;

	if (arg_arch  != NULL) {
		size = (unsigned long)arg_arch - (unsigned long)arg;
		*arg_name = malloc(size + 1);
		if (*arg_name == NULL)
			exit(EXIT_FAILURE);
		(*arg_name)[size] = 0;
		memcpy(*arg_name, arg, size);

		//identify architecture
		if ((only_64bit != NULL) && (only_32bit != NULL)) {
			if ((strcmp(arg_arch + 1, "64") == 0)) {
				*only_64bit = TRUE;
				*only_32bit = FALSE;
			} else if ((strcmp(arg_arch + 1,"32") == 0)) {
				*only_64bit = FALSE;
				*only_32bit = TRUE;
			} else {
				outputerr("Unknown bit width (%s). Choose 32, or 64.\n", arg);
				exit(EXIT_FAILURE);
			}
		}
	} else {
		*arg_name = (char*)arg;//castaway const.
	}
}

void clear_check_user_specified_arch(const char *arg, char **arg_name)
{
	//Release memory only if we have allocated it
	if (((char *)arg) != *arg_name) {
		free(*arg_name);
		*arg_name = NULL;
	}
}

void toggle_syscall(const char *arg, bool state)
{
	int specific_syscall = 0;
	char * arg_name = NULL;

	if (biarch == TRUE) {
		toggle_syscall_biarch(arg, state);
		return;
	}

	/* non-biarch case. */
	check_user_specified_arch(arg, &arg_name, NULL, NULL); //We do not care about arch here, just to get rid of arg flags.
	specific_syscall = search_syscall_table(syscalls, max_nr_syscalls, arg_name);
	toggle_syscall_n(specific_syscall, state, arg, arg_name);
	clear_check_user_specified_arch(arg, &arg_name);
}

void deactivate_disabled_syscalls(void)
{
	output(0, "Disabling syscalls marked as disabled by command line options\n");

	if (biarch == TRUE)
		deactivate_disabled_syscalls_biarch();
	else
		deactivate_disabled_syscalls_uniarch();
}

void show_state(unsigned int state)
{
	if (state)
		outputstd("Enabled");
	else
		outputstd("Disabled");
}

void dump_syscall_tables(void)
{
	if (biarch == TRUE)
		dump_syscall_tables_biarch();
	else
		dump_syscall_tables_uniarch();
}

/*
 * This changes the pointers in the table 'from' to be copies in
 * shared mmaps across all children.  We do this so that a child can
 * modify the flags field (adding AVOID for eg) and have other processes see the change.
 */
static struct syscalltable * copy_syscall_table(struct syscalltable *from, unsigned int nr)
{
	unsigned int n;
	struct syscall *copy;

	copy = alloc_shared(nr * sizeof(struct syscall));
	if (copy == NULL)
		exit(EXIT_FAILURE);

	for (n = 0; n < nr; n++) {
    if (from[n].entry != NULL) {
      memcpy(copy + n , from[n].entry, sizeof(struct syscall));
      copy[n].number = n;
      copy[n].active_number = 0;
      from[n].entry = &copy[n];
    } else {
      from[n].entry = malloc(sizeof(struct syscall));
      memset(from[n].entry, 0, sizeof(struct syscall));
      from[n].entry->flags = TO_BE_DEACTIVATED;
    }
	}
	return from;
}

void select_syscall_tables(void)
{
#ifdef ARCH_IS_BIARCH
	syscalls_64bit = copy_syscall_table(SYSCALLS64, ARRAY_SIZE(SYSCALLS64));
	syscalls_32bit = copy_syscall_table(SYSCALLS32, ARRAY_SIZE(SYSCALLS32));

	max_nr_64bit_syscalls = ARRAY_SIZE(SYSCALLS64);
	max_nr_32bit_syscalls = ARRAY_SIZE(SYSCALLS32);
	biarch = TRUE;
#else
	syscalls = copy_syscall_table(SYSCALLS, ARRAY_SIZE(SYSCALLS));
	max_nr_syscalls = ARRAY_SIZE(SYSCALLS);
#endif
}

int setup_syscall_group(unsigned int group)
{
	if (biarch == TRUE)
		return setup_syscall_group_biarch(group);
	else
		return setup_syscall_group_uniarch(group);
}

const char * print_syscall_name(unsigned int callno, bool is32bit)
{
	const struct syscalltable *table;
	unsigned int max;

	if (biarch == FALSE) {
		max = max_nr_syscalls;
		table = syscalls;
	} else {
		if (is32bit == FALSE) {
			max = max_nr_64bit_syscalls;
			table = syscalls_64bit;
		} else {
			max = max_nr_32bit_syscalls;
			table = syscalls_32bit;
		}
	}

	if (callno >= max) {
		outputstd("Bogus syscall number in %s (%u)\n", __func__, callno);
		return "invalid-syscall";
	}

	return table[callno].entry->name;
}

void display_enabled_syscalls(void)
{
	if (biarch == TRUE)
		display_enabled_syscalls_biarch();
	else
		display_enabled_syscalls_uniarch();
}

/* If we want just network sockets, don't bother with VM/VFS syscalls */
bool is_syscall_net_related(const struct syscalltable *table, unsigned int num)
{
	unsigned int i;

	if (no_files == FALSE)
		return TRUE;

	if (table[num].entry->group == GROUP_VM)
		return FALSE;
	if (table[num].entry->group == GROUP_VFS)
		return FALSE;

	for (i = 0; i < table[num].entry->num_args; i++) {
		switch (i) {
		case 0:	if (table[num].entry->arg1type == ARG_PATHNAME)
				return FALSE;
			break;
		case 1:	if (table[num].entry->arg2type == ARG_PATHNAME)
				return FALSE;
			break;
		case 2:	if (table[num].entry->arg3type == ARG_PATHNAME)
				return FALSE;
			break;
		case 3:	if (table[num].entry->arg4type == ARG_PATHNAME)
				return FALSE;
			break;
		case 4:	if (table[num].entry->arg5type == ARG_PATHNAME)
				return FALSE;
			break;
		case 5:	if (table[num].entry->arg6type == ARG_PATHNAME)
				return FALSE;
			break;
		default:
			BUG("impossible!\n");
		}
	}

	return TRUE;
}

void disable_non_net_syscalls(void)
{
	output(0, "Disabling non networking related syscalls\n");

	if (biarch == TRUE)
		disable_non_net_syscalls_biarch();
	else
		disable_non_net_syscalls_uniarch();

	deactivate_disabled_syscalls();
}

void enable_random_syscalls(void)
{
	unsigned int i;

	if (random_selection_num == 0) {
		outputerr("-r 0 syscalls ? what?\n");
		exit(EXIT_FAILURE);
	}

	if (biarch == TRUE) {
		if ((random_selection_num > max_nr_64bit_syscalls) && do_64_arch) {
			outputerr("-r val %d out of range (1-%d)\n", random_selection_num, max_nr_64bit_syscalls);
			exit(EXIT_FAILURE);
		}
	} else {
		if (random_selection_num > max_nr_syscalls) {
			outputerr("-r val %d out of range (1-%d)\n", random_selection_num, max_nr_syscalls);
			exit(EXIT_FAILURE);
		}
	}

	outputerr("Enabling %d random syscalls\n", random_selection_num);

	for (i = 0; i < random_selection_num; i++) {
		if (biarch == TRUE)
			enable_random_syscalls_biarch();
		else
			enable_random_syscalls_uniarch();
	}
}
