#ifndef _PARAMS_H
#define _PARAMS_H 1

#include "types.h"
#include "net.h"

#define TAINT_PROPRIETARY_MODULE        0
#define TAINT_FORCED_MODULE             1
#define TAINT_UNSAFE_SMP                2
#define TAINT_FORCED_RMMOD              3
#define TAINT_MACHINE_CHECK             4
#define TAINT_BAD_PAGE                  5
#define TAINT_USER                      6
#define TAINT_DIE                       7
#define TAINT_OVERRIDDEN_ACPI_TABLE     8
#define TAINT_WARN                      9
#define TAINT_CRAP                      10
#define TAINT_FIRMWARE_WORKAROUND       11
#define TAINT_OOT_MODULE                12

/* command line args. */
void parse_args(int argc, char *argv[]);

extern bool debug;

extern bool do_32_arch;
extern bool do_64_arch;

extern bool do_specific_syscall;
extern bool do_exclude_syscall;
extern unsigned int specific_proto;
extern bool do_specific_proto;
extern char *specific_proto_optarg;
extern bool no_protos[TRINITY_PF_MAX];
extern bool dopause;
extern bool show_syscall_list;
extern bool show_ioctl_list;
extern unsigned char quiet_level;
extern bool verbose;
extern bool monochrome;
extern bool dangerous;
extern bool do_syslog;
extern bool logging;
extern unsigned char desired_group;
extern bool user_set_seed;
extern char *victim_path;
extern bool no_files;
extern bool random_selection;
extern unsigned int random_selection_num;

extern int kernel_taint_initial;
extern int kernel_taint_mask;
extern bool kernel_taint_param_occured;

#endif	/* _PARAMS_H */
