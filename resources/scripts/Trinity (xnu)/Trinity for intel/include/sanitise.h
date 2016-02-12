#ifndef _SANITISE_H
#define _SANITISE_H 1

#include "syscall.h"

void sanitise_mmap(int childno);
void sanitise_rt_sigaction(int childno);
void sanitise_prctl(int childno);
void sanitise_perf_event_open(int childno);

void generic_sanitise(int childno);

unsigned long get_interesting_value(void);
unsigned int get_interesting_32bit_value(void);
unsigned long get_reg(void);

void *get_address(void);
void *get_non_null_address(void);
unsigned long find_previous_arg_address(unsigned int argnum, unsigned int call, int childno);
struct iovec * alloc_iovec(unsigned int num);

unsigned long get_len(void);
unsigned int get_pid(void);
const char * get_filename(void);
int get_random_fd(void);
const char * generate_pathname(void);

void gen_unicode_page(char *page);

#endif	/* _SANITISE_H */
