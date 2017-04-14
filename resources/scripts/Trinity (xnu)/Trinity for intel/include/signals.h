#ifndef _SIGNALS_H
#define _SIGNALS_H 1

#include <setjmp.h>

extern jmp_buf ret_jump;
void mask_signals_child(void);
void setup_main_signals(void);

#endif	/* _SIGNALS_H */
