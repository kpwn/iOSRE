#ifndef _ARCH_H
#define _ARCH_H 1

#ifdef __APPLE__
#include "arch-bsd.h"
#else


#ifdef __x86_64__
#include "arch-x86-64.h"
#endif

#ifdef __i386__
#include "arch-i386.h"
#endif

#ifdef __powerpc__
#include "arch-ppc.h"
#endif

#ifdef __ia64__
#include "arch-ia64.h"
#endif

#ifdef __sparc__
#include "arch-sparc.h"
#endif

#ifdef __s390__
#include "arch-s390.h"
#endif

#ifdef __arm__
#include "arch-arm.h"
#endif

#ifdef __mips__
#include "arch-mips.h"
#endif

#ifdef __sh__
#include "arch-sh.h"
#endif

#ifdef __alpha__
#include "arch-alpha.h"
#endif

#ifdef __aarch64__
#include "arch-aarch64.h"
#endif

#ifdef __hppa__
#include "arch-parisc.h"
#endif

#endif

#ifndef SYSCALL_OFFSET
#define SYSCALL_OFFSET 0
#endif


#define PAGE_MASK (~(page_size - 1))
extern unsigned int page_size;

#endif  /* _ARCH_H */
