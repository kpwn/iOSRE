#ifndef _TRINITY_H
#define _TRINITY_H 1

#include "types.h"

extern unsigned int num_online_cpus;

#define UNLOCKED 0
#define LOCKED 1

#define __unused__ __attribute((unused))

extern char *progname;

void * alloc_shared(unsigned int size);

void do_main_loop(void);

extern bool biarch;

int check_tainted(void);

void init_watchdog(void);
unsigned int check_if_fd(unsigned int child);

extern unsigned int user_specified_children;

void regenerate(void);

#define UNUSED(x) (void)(x)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define max(x, y) ((x) >= (y) ? (x) : (y))
#define min(x, y) ((x) <= (y) ? (x) : (y))

#ifndef offsetof
# define offsetof(type, member)	((size_t) &((type *) 0)->member)
#endif

#define MAX_ERRNO       4095
#define IS_ERR_VALUE(x) ((x) >= (unsigned long)-MAX_ERRNO)
static inline long IS_ERR(unsigned long x)
{
	return IS_ERR_VALUE(x);
}


#endif	/* _TRINITY_H */
