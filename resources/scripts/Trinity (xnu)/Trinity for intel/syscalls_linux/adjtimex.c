/*
 * SYSCALL_DEFINE1(adjtimex, struct timex __user *, txc_p
 *
 * On success, adjtimex() returns the clock state: */

#define TIME_OK   0 /* clock synchronized */
#define TIME_INS  1 /* insert leap second */
#define TIME_DEL  2 /* delete leap second */
#define TIME_OOP  3 /* leap second in progress */
#define TIME_WAIT 4 /* leap second has occurred */
#define TIME_BAD  5 /* clock not synchronized */

 /* On failure, adjtimex() returns -1 and sets errno. */

#include "sanitise.h"

struct syscall syscall_adjtimex = {
	.name = "adjtimex",
	.num_args = 1,
	.arg1name = "txc_p",
	.arg1type = ARG_ADDRESS,
};
