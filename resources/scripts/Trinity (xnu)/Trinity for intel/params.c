#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "trinity.h"	// progname
#include "random.h"
#include "syscall.h"
#include "log.h"
#include "net.h"
#include "params.h"
#include "protocols.h"
#include "tables.h"

#define TAINT_NAME_LEN 32

bool debug = FALSE;

bool do_specific_syscall = FALSE;
bool do_exclude_syscall = FALSE;

bool do_32_arch = TRUE;
bool do_64_arch = TRUE;

unsigned int specific_proto = 0;
unsigned int user_specified_children = 0;

bool do_specific_proto = FALSE;
bool no_protos[TRINITY_PF_MAX];

bool dopause = FALSE;
bool show_syscall_list = FALSE;
bool show_ioctl_list = FALSE;
unsigned char quiet_level = 0;
bool verbose = FALSE;
bool monochrome = FALSE;
bool dangerous = FALSE;
bool logging = TRUE;
bool do_syslog = FALSE;
bool random_selection = FALSE;
unsigned int random_selection_num;
bool no_files = FALSE;

bool user_set_seed = FALSE;

unsigned char desired_group = GROUP_NONE;

char *specific_proto_optarg;

char *victim_path;

int kernel_taint_initial = 0;
int kernel_taint_mask = 0xFFFFFFFF;
bool kernel_taint_param_occured = FALSE;

static void usage(void)
{
	outputerr("%s\n", progname);
	outputerr(" --arch, -a: selects syscalls for the specified architecture (32 or 64). Both by default.\n");
	outputerr(" --children,-C: specify number of child processes\n");
	outputerr(" --debug,-D: enable debug\n");
	outputerr(" --exclude,-x: don't call a specific syscall\n");
	outputerr(" --group,-g: only run syscalls from a certain group (So far just 'vm').\n");
	outputerr(" --ioctls,-I: list all ioctls.\n");
	outputerr(" --kernel_taint, -T: controls which kernel taint flags should be considered, for more details refer to README file. \n");
	outputerr(" --list,-L: list all syscalls known on this architecture.\n");
	outputerr(" --logging,-l: (off=disable logging).\n");
	outputerr(" --monochrome,-m: don't output ANSI codes\n");
	outputerr(" --no_files,-n: Only pass sockets as fd's, not files\n");
	outputerr(" --proto,-P: specify specific network protocol for sockets.\n");
	outputerr(" --no_proto,-E: specify network protocols to be excluded from testing.\n");
	outputerr(" --quiet,-q: less output.\n");
	outputerr(" --random,-r#: pick N syscalls at random and just fuzz those\n");
	outputerr(" --syslog,-S: log important info to syslog. (useful if syslog is remote)\n");
	outputerr(" --verbose,-v: increase output verbosity.\n");
	outputerr(" --victims,-V: path to victim files.\n");
	outputerr("\n");
	outputerr(" -c#,@: target specific syscall (takes syscall name as parameter and optionally 32 or 64 as bit-width. Default:both).\n");
	outputerr(" -N#: do # syscalls then exit.\n");
	outputerr(" -p:  pause after syscall.\n");
	outputerr(" -s#: use # as random seed.\n");
	exit(EXIT_SUCCESS);
}

static const struct option longopts[] = {
	{ "children", required_argument, NULL, 'C' },
	{ "dangerous", no_argument, NULL, 'd' },
	{ "debug", no_argument, NULL, 'D' },
	{ "exclude", required_argument, NULL, 'x' },
	{ "group", required_argument, NULL, 'g' },
	{ "kernel_taint", required_argument, NULL, 'T' },
	{ "help", no_argument, NULL, 'h' },
	{ "list", no_argument, NULL, 'L' },
	{ "ioctls", no_argument, NULL, 'I' },
	{ "logging", required_argument, NULL, 'l' },
	{ "monochrome", no_argument, NULL, 'm' },
	{ "no_files", no_argument, NULL, 'n' },
	{ "proto", required_argument, NULL, 'P' },
	{ "no_proto", required_argument, NULL, 'E' },
	{ "random", required_argument, NULL, 'r' },
	{ "quiet", no_argument, NULL, 'q' },
	{ "syslog", no_argument, NULL, 'S' },
	{ "victims", required_argument, NULL, 'V' },
	{ "verbose", no_argument, NULL, 'v' },
	{ "arch", required_argument, NULL, 'a' },
	{ NULL, 0, NULL, 0 } };

static void toggle_taint_flag(int bit) {
	kernel_taint_mask |= (1 << bit);
}

static void toggle_taint_flag_by_name(char *beg, char *end) {
	char flagname[TAINT_NAME_LEN];
	char *name;
	int maxlen;

	if (end == NULL) {
		name = beg;
	} else {
		name = flagname;
		maxlen = end - beg;
		if (maxlen > (TAINT_NAME_LEN - 1))
			maxlen = TAINT_NAME_LEN - 1;
		strncpy(flagname, beg, maxlen);
		flagname[maxlen] = 0;
	}

	if (strcmp(name,"PROPRIETARY_MODULE") == 0)
		toggle_taint_flag(TAINT_PROPRIETARY_MODULE);
	else if (strcmp(name,"FORCED_MODULE") == 0)
		toggle_taint_flag(TAINT_FORCED_MODULE);
	else if (strcmp(name,"UNSAFE_SMP") == 0)
		toggle_taint_flag(TAINT_UNSAFE_SMP);
	else if (strcmp(name,"FORCED_RMMOD") == 0)
		toggle_taint_flag(TAINT_FORCED_RMMOD);
	else if (strcmp(name,"MACHINE_CHECK") == 0)
		toggle_taint_flag(TAINT_MACHINE_CHECK);
	else if (strcmp(name,"BAD_PAGE") == 0)
		toggle_taint_flag(TAINT_BAD_PAGE);
	else if (strcmp(name,"USER") == 0)
		toggle_taint_flag(TAINT_USER);
	else if (strcmp(name,"DIE") == 0)
		toggle_taint_flag(TAINT_DIE);
	else if (strcmp(name,"OVERRIDDEN_ACPI_TABLE") == 0)
		toggle_taint_flag(TAINT_OVERRIDDEN_ACPI_TABLE);
	else if (strcmp(name,"WARN") == 0)
		toggle_taint_flag(TAINT_WARN);
	else if (strcmp(name,"CRAP") == 0)
		toggle_taint_flag(TAINT_CRAP);
	else if (strcmp(name,"FIRMWARE_WORKAROUND") == 0)
		toggle_taint_flag(TAINT_FIRMWARE_WORKAROUND);
	else if (strcmp(name,"OOT_MODULE") == 0)
		toggle_taint_flag(TAINT_OOT_MODULE);
	else {
		outputerr("Unrecognizable kernel taint flag \"%s\".\n", name);
		exit(EXIT_FAILURE);
	}
}

static void process_taint_arg(char *taintarg) {
	char *beg, *end;

	if (kernel_taint_param_occured == FALSE) {
		kernel_taint_param_occured = TRUE;
		kernel_taint_mask = 0; //We now only care about flags that user specified.
	}

	beg = taintarg;
	end = strchr(beg, ',');
	while(end != NULL) {
		toggle_taint_flag_by_name(beg,end);
		beg = end + 1;
		end = strchr(beg, ',');
	}
	toggle_taint_flag_by_name(beg,end);
}

void parse_args(int argc, char *argv[])
{
	int opt;

	while ((opt = getopt_long(argc, argv, "a:c:C:dDg:hIl:LN:mnP:E:pqr:s:T:SV:vx:", longopts, NULL)) != -1) {
		switch (opt) {
		default:
			if (opt == '?')
				exit(EXIT_FAILURE);
			else
				outputstd("opt:%c\n", opt);
			return;

		case '\0':
			return;

		case 'c':
			/* syscalls are all disabled at this point. enable the syscall we care about. */
			do_specific_syscall = TRUE;
			toggle_syscall(optarg, TRUE);
			break;

		case 'a':
			/* One of the architectures selected*/
			do_32_arch = FALSE;
			do_64_arch = FALSE;
			if (strcmp(optarg, "64") == 0)
				do_64_arch = TRUE;
			else if (strcmp(optarg, "32") == 0)
				do_32_arch = TRUE;
			else
				exit(EXIT_FAILURE);

			break;

		case 'C':
			user_specified_children = strtoll(optarg, NULL, 10);
			break;

		case 'd':
			dangerous = 1;
			break;

		case 'D':
			debug = 1;
			break;

		case 'g':
			if (!strcmp(optarg, "vm"))
				desired_group = GROUP_VM;
			if (!strcmp(optarg, "vfs"))
				desired_group = GROUP_VFS;
			break;

		/* Show help */
		case 'h':
			usage();
			exit(EXIT_SUCCESS);

		case 'I':
			show_ioctl_list = TRUE;
			break;

		case 'l':
			if (!strcmp(optarg, "off"))
				logging = 0;
			break;

		case 'L':
			show_syscall_list = TRUE;
			break;

		case 'm':
			monochrome = TRUE;
			break;

		case 'n':
			no_files = TRUE;
			break;

		/* Set number of syscalls to do */
		case 'N':
			syscalls_todo = strtoll(optarg, NULL, 10);
			break;

		/* Pause after each syscall */
		case 'p':
			dopause = 1;
			break;

		case 'P':
			do_specific_proto = 1;
			specific_proto = strtol(optarg, NULL, 10);
			specific_proto_optarg = optarg;
			break;

		case 'E':
			//parse_exclude_protos(optarg);
			break;

		case 'q':
			quiet_level++;
			break;

		case 'r':
			if (do_exclude_syscall == TRUE) {
				outputerr("-r needs to be before any -x options.\n");
				exit(EXIT_FAILURE);
			}
			random_selection = 1;
			random_selection_num = strtol(optarg, NULL, 10);
			break;

		/* Set seed */
		case 's':
			seed = strtol(optarg, NULL, 10);
			user_set_seed = TRUE;
			break;


		case 'S':
			do_syslog = TRUE;
			break;

		case 'T':
			//Load mask for kernel taint flags.
			process_taint_arg(optarg);
			break;

		case 'v':
			verbose = TRUE;
			break;

		case 'V':
			victim_path = strdup(optarg);
			//FIXME: Later, allow for multiple victim files
			break;

		case 'x':
			do_exclude_syscall = TRUE;
			toggle_syscall(optarg, FALSE);
			break;
		}
	}
	if (quiet_level > MAX_LOGLEVEL)
		quiet_level = MAX_LOGLEVEL;

	quiet_level = MAX_LOGLEVEL - quiet_level;
}
