#ifndef _TRINITY_TABLES_H
#define _TRINITY_TABLES_H 1

#include "types.h"
#include "syscall.h"

extern const struct syscalltable *syscalls;
extern const struct syscalltable *syscalls_32bit;
extern const struct syscalltable *syscalls_64bit;

extern unsigned long syscalls_todo;
extern unsigned int max_nr_syscalls;
extern unsigned int max_nr_32bit_syscalls;
extern unsigned int max_nr_64bit_syscalls;

extern bool use_32bit;
extern bool use_64bit;

void validate_specific_syscall(const struct syscalltable *table, int call);
void activate_syscall_in_table(unsigned int calln, unsigned int *nr_active, const struct syscalltable *table, int *active_syscall);
void deactivate_syscall_in_table(unsigned int calln, unsigned int *nr_active, const struct syscalltable *table, int *active_syscall);

void check_user_specified_arch(const char *arg, char **arg_name, bool *only_64bit, bool *only_32bit);
void clear_check_user_specified_arch(const char *arg, char **arg_name);

void select_syscall_tables(void);
int search_syscall_table(const struct syscalltable *table, unsigned int nr_syscalls, const char *arg);

void mark_all_syscalls_active(void);
void mark_all_syscalls_active_uniarch(void);
void mark_all_syscalls_active_biarch(void);

void toggle_syscall(const char *arg, bool state);
void toggle_syscall_biarch_n(int calln, const struct syscalltable *table, bool onlyflag, bool doflag, bool state, void (*activate)(unsigned int), int arch_bits, const char *arg_name);
void toggle_syscall_biarch(const char *arg, bool state);
void toggle_syscall_n(int calln, bool state, const char *arg, const char *arg_name);

void activate_syscall(unsigned int calln);
void activate_syscall32(unsigned int calln);
void activate_syscall64(unsigned int calln);
void deactivate_syscall(unsigned int calln);
void deactivate_syscall32(unsigned int calln);
void deactivate_syscall64(unsigned int calln);

void show_state(unsigned int state);
void dump_syscall_tables(void);
void dump_syscall_tables_uniarch(void);
void dump_syscall_tables_biarch(void);

int setup_syscall_group(unsigned int desired_group);
int setup_syscall_group_uniarch(unsigned int desired_group);
int setup_syscall_group_biarch(unsigned int desired_group);

int validate_syscall_tables(void);
bool no_syscalls_enabled(void);
int validate_syscall_table_64(void);
int validate_syscall_table_32(void);
void sanity_check_tables(void);
const char * print_syscall_name(unsigned int callno, bool is32bit);

void enable_random_syscalls(void);
void enable_random_syscalls_biarch(void);
void enable_random_syscalls_uniarch(void);

int validate_specific_syscall_silent(const struct syscalltable *table, int call);
void deactivate_disabled_syscalls(void);
void deactivate_disabled_syscalls_biarch(void);
void deactivate_disabled_syscalls_uniarch(void);
void count_syscalls_enabled(void);

void display_enabled_syscalls(void);
void display_enabled_syscalls_uniarch(void);
void display_enabled_syscalls_biarch(void);

void disable_non_net_syscalls(void);
void disable_non_net_syscalls_uniarch(void);
void disable_non_net_syscalls_biarch(void);

bool is_syscall_net_related(const struct syscalltable *table, unsigned int num);

void init_syscalls(void);
void init_syscalls_biarch(void);
void init_syscalls_uniarch(void);

#define for_each_32bit_syscall(i) \
	for (i = 0; i < max_nr_32bit_syscalls; i++)
#define for_each_64bit_syscall(i) \
	for (i = 0; i < max_nr_64bit_syscalls; i++)
#define for_each_syscall(i) \
	for (i = 0; i < max_nr_syscalls; i++)

#endif	/* _TRINITY_TABLES_H */
