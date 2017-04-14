#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "params.h"	// logging, monochrome, quiet_level
#include "shm.h"
#include "pids.h"
#include "log.h"
#include "arch.h" //PAGE_MASK
#include "maps.h" //pages
#include "syscall.h" //syscalls
#include "tables.h"

#define BUFSIZE 1024

FILE *mainlogfile;
bool logfiles_opened = FALSE;

void open_logfiles(void)
{
	unsigned int i;
	char *logfilename;

	logfilename = malloc(64);
	sprintf(logfilename, "trinity.log");
	unlink(logfilename);
	mainlogfile = fopen(logfilename, "a");
	if (!mainlogfile) {
		outputerr("## couldn't open logfile %s\n", logfilename);
		exit(EXIT_FAILURE);
	}

	for_each_pidslot(i) {
		sprintf(logfilename, "trinity-child%d.log", i);
		unlink(logfilename);
		shm->logfiles[i] = fopen(logfilename, "a");
		if (!shm->logfiles[i]) {
			outputerr("## couldn't open logfile %s\n", logfilename);
			exit(EXIT_FAILURE);
		}
	}
	free(logfilename);
	logfiles_opened = TRUE;
}

void close_logfiles(void)
{
	unsigned int i;

	for_each_pidslot(i)
		if (shm->logfiles[i] != NULL)
			fclose(shm->logfiles[i]);
}

static FILE * find_logfile_handle(void)
{
	pid_t pid;
	int i;
	unsigned int j;

	pid = getpid();
	if (pid == initpid)
		return mainlogfile;

	if (pid == shm->mainpid)
		return mainlogfile;

	if (pid == watchdog_pid)
		return mainlogfile;

	i = find_pid_slot(pid);
	if (i != PIDSLOT_NOT_FOUND)
		return shm->logfiles[i];
	else {
		/* try one more time. FIXME: This is awful. */
		sleep(1);
		i = find_pid_slot(pid);
		if (i != PIDSLOT_NOT_FOUND)
			return shm->logfiles[i];

		outputerr("## Couldn't find logfile for pid %d\n", pid);
		dump_pid_slots();
		outputerr("## Logfiles for pids: ");
		for_each_pidslot(j)
			outputerr("%p ", shm->logfiles[j]);
		outputerr("\n");
	}
	return NULL;
}

unsigned int highest_logfile(void)
{
	FILE *file;
	int ret;

	if (logging == FALSE)
		return 0;

	file = shm->logfiles[shm->max_children - 1];
	ret = fileno(file);

	return ret;
}

void synclogs(void)
{
	unsigned int i;
	int fd, ret;

	if (logging == FALSE)
		return;

	for_each_pidslot(i) {
		ret = fflush(shm->logfiles[i]);
		if (ret == EOF) {
			outputerr("## logfile flushing failed! %s\n", strerror(errno));
			continue;
		}

		fd = fileno(shm->logfiles[i]);
		if (fd != -1) {
			ret = fsync(fd);
			if (ret != 0)
				outputerr("## fsyncing logfile %d failed. %s\n", i, strerror(errno));
		}
	}

	(void)fflush(mainlogfile);
	fsync(fileno(mainlogfile));
}

static void output_arg(unsigned int call, unsigned int argnum, const char *name, unsigned long reg, int type, FILE *fd, bool mono)
{
	if (syscalls[call].entry->num_args >= argnum) {
		if (!name)
			return;

		if (argnum != 1) {
			CRESETFD
			fprintf(fd, ", ");
		}
		if (name)
			fprintf(fd, "%s=", name);

		switch (type) {
		case ARG_PATHNAME:
			fprintf(fd, "\"%s\"", (char *) reg);
			break;
		case ARG_PID:
		case ARG_FD:
			CRESETFD
			fprintf(fd, "%ld", reg);
			break;
		case ARG_MODE_T:
			CRESETFD
			fprintf(fd, "%o", (mode_t) reg);
			break;
		case ARG_UNDEFINED:
		case ARG_LEN:
		case ARG_ADDRESS:
		case ARG_NON_NULL_ADDRESS:
		case ARG_RANGE:
		case ARG_OP:
		case ARG_LIST:
		case ARG_RANDPAGE:
		case ARG_CPU:
		case ARG_RANDOM_LONG:
		case ARG_IOVEC:
		case ARG_IOVECLEN:
		case ARG_SOCKADDR:
		case ARG_SOCKADDRLEN:
		default:
			if (reg > 8 * 1024)
				fprintf(fd, "0x%lx", reg);
			else
				fprintf(fd, "%ld", reg);
			CRESETFD
			break;
		}
		if (reg == (((unsigned long)page_zeros) & PAGE_MASK))
			fprintf(fd, "[page_zeros]");
		if (reg == (((unsigned long)page_rand) & PAGE_MASK))
			fprintf(fd, "[page_rand]");
		if (reg == (((unsigned long)page_0xff) & PAGE_MASK))
			fprintf(fd, "[page_0xff]");
		if (reg == (((unsigned long)page_allocs) & PAGE_MASK))
			fprintf(fd, "[page_allocs]");
	}
}

static FILE *robust_find_logfile_handle(void)
{
	unsigned int j;
	FILE *handle = NULL;

	if ((logging == TRUE) && (logfiles_opened)) {
		handle = find_logfile_handle();
		if (!handle) {
			outputerr("## child logfile handle was null logging to main!\n");
			(void)fflush(stdout);
			for_each_pidslot(j)
				shm->logfiles[j] = mainlogfile;
			sleep(5);
			handle = find_logfile_handle();
		}
	}
	return handle;
}

/*
 * level defines whether it gets displayed to the screen with printf.
 * (it always logs).
 *   0 = everything, even all the registers
 *   1 = Watchdog prints syscall count
 *   2 = Just the reseed values
 *
 */
void output(unsigned char level, const char *fmt, ...)
{
	va_list args;
	int n;
	FILE *handle;
	unsigned int len, i, j;
	pid_t pid;
	char outputbuf[BUFSIZE];
	char monobuf[BUFSIZE];
	char *prefix = NULL;
	char watchdog_prefix[]="[watchdog]";
	char init_prefix[]="[init]";
	char main_prefix[]="[main]";
	char child_prefix[]="[childNN:1234567890]";
	unsigned int slot;

	if (logging == FALSE && level >= quiet_level)
		return;

	/* prefix preparation */
	pid = getpid();
	if (pid == watchdog_pid)
		prefix = watchdog_prefix;

	if (pid == initpid)
		prefix = init_prefix;

	if (pid == shm->mainpid)
		prefix = main_prefix;

	if (prefix == NULL) {
		slot = find_pid_slot(pid);
		sprintf(child_prefix, "[child%d:%d]", slot, pid);
		prefix = child_prefix;
	}

	/* formatting output */
	va_start(args, fmt);
	n = vsnprintf(outputbuf, sizeof(outputbuf), fmt, args);
	va_end(args);
	if (n < 0) {
		outputerr("## Something went wrong in output() [%d]\n", n);
		exit(EXIT_FAILURE);
	}

	/* stdout output if needed */
	if (quiet_level > level) {
		printf("%s %s", prefix, outputbuf);
		(void)fflush(stdout);
	}

	/* go on with file logs only if enabled */
	if (logging == FALSE)
		return;

	handle = robust_find_logfile_handle();
	if (!handle)
		return;

	/* If we've specified monochrome, we can just dump the buffer into
	 * the logfile as is, because there shouldn't be any ANSI codes
	 * in the buffer to be stripped out. */
	if (monochrome == FALSE) {
		/* copy buffer, sans ANSI codes */
		len = strlen(outputbuf);
		for (i = 0, j = 0; (i < len) && (i + 2 < BUFSIZE) && (j < BUFSIZE); i++) {
			if (outputbuf[i] == '') {
				if (outputbuf[i + 2] == '1')
					i += 6;	// ANSI_COLOUR
				else
					i += 3;	// ANSI_RESET
			} else {
				monobuf[j] = outputbuf[i];
				j++;
			}
		}
		monobuf[j] = '\0';
		fprintf(handle, "%s %s", prefix, monobuf);
	} else {
		fprintf(handle, "%s %s", prefix, outputbuf);
	}

	(void)fflush(handle);
}

/*
* Used as a way to consolidated all printf calls if someones one to redirect it to somewhere else.
* note: this function ignores quiet_level since it main purpose is error output.
*/
void outputerr(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

void outputstd(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
}

static void output_syscall_prefix_to_fd(const unsigned int childno, const pid_t pid, const unsigned int syscallno, FILE *fd, bool mono)
{
	fprintf(fd, "[child%d:%d] [%ld] %s", childno, pid, shm->child_syscall_count[childno],
			(shm->do32bit[childno] == TRUE) ? "[32BIT] " : "");

	if (syscallno > max_nr_syscalls)
		fprintf(fd, "%u", syscallno);
	else
		fprintf(fd, "%s", syscalls[syscallno].entry->name);

	CRESETFD
	fprintf(fd, "(");
	output_arg(syscallno, 1, syscalls[syscallno].entry->arg1name, shm->a1[childno],
			syscalls[syscallno].entry->arg1type, fd, mono);
	output_arg(syscallno, 2, syscalls[syscallno].entry->arg2name, shm->a2[childno],
			syscalls[syscallno].entry->arg2type, fd, mono);
	output_arg(syscallno, 3, syscalls[syscallno].entry->arg3name, shm->a3[childno],
			syscalls[syscallno].entry->arg3type, fd, mono);
	output_arg(syscallno, 4, syscalls[syscallno].entry->arg4name, shm->a4[childno],
			syscalls[syscallno].entry->arg4type, fd, mono);
	output_arg(syscallno, 5, syscalls[syscallno].entry->arg5name, shm->a5[childno],
			syscalls[syscallno].entry->arg5type, fd, mono);
	output_arg(syscallno, 6, syscalls[syscallno].entry->arg6name, shm->a6[childno],
			syscalls[syscallno].entry->arg6type, fd, mono);
	CRESETFD
	fprintf(fd, ") ");
}

/* This function is always called from a fuzzing child. */
void output_syscall_prefix(const unsigned int childno, const unsigned int syscallno)
{
	FILE *log_handle;
	pid_t pid;

	/* Exit if should not continue at all. */
	if (logging == FALSE && quiet_level < MAX_LOGLEVEL)
		return;
	pid = getpid();

	/* Find the log file handle */
	log_handle = robust_find_logfile_handle();

	/* do not output any ascii control symbols to files */
	if ((logging == TRUE) && (log_handle != NULL))
		output_syscall_prefix_to_fd(childno, pid, syscallno, log_handle, TRUE);

	/* Output to stdout only if -q param is not specified */
	if (quiet_level == MAX_LOGLEVEL)
		output_syscall_prefix_to_fd(childno, pid, syscallno, stdout, monochrome);
}

static void output_syscall_postfix_err(unsigned long ret, int errno_saved, FILE *fd, bool mono)
{
	REDFD
	fprintf(fd, "= %ld (%s)", ret, strerror(errno_saved));
	CRESETFD
	fprintf(fd, "\n");
}

static void output_syscall_postfix_success(unsigned long ret, FILE *fd, bool mono)
{
	GREENFD
	if ((unsigned long)ret > 10000)
		fprintf(fd, "= 0x%lx", ret);
	else
		fprintf(fd, "= %ld", ret);
	CRESETFD
	fprintf(fd, "\n");
}

void output_syscall_postfix(unsigned long ret, int errno_saved, bool err)
{
	FILE *log_handle;

	/* Exit if should not continue at all. */
	if (logging == FALSE && quiet_level < MAX_LOGLEVEL)
		return;

	/* Find the log file handle */
	log_handle = robust_find_logfile_handle();

	if (err) {
		if ((logging == TRUE) && (log_handle != NULL))
			output_syscall_postfix_err(ret, errno_saved, log_handle, TRUE);
		if (quiet_level == MAX_LOGLEVEL)
			output_syscall_postfix_err(ret, errno_saved, stdout, monochrome);
	} else {
		if ((logging == TRUE) && (log_handle != NULL))
			output_syscall_postfix_success(ret, log_handle, TRUE);
		if (quiet_level == MAX_LOGLEVEL)
			output_syscall_postfix_success(ret, stdout, monochrome);
	}
}

void debugf(const char *fmt, ...)
{
	va_list args;

	if (debug == TRUE) {
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}
}
