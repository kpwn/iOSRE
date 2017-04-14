#ifndef _ARCH_SYSCALLS_H
#define _ARCH_SYSCALLS_H 1

#ifdef __APPLE__
#include "syscalls-bsd.h"

#else

#ifdef __x86_64__
#include "syscalls-x86_64.h"
#include "syscalls-i386.h"
#endif
#ifdef __i386__
#include "syscalls-i386.h"
#endif
#ifdef __powerpc__
#include "syscalls-ppc.h"
#endif
#ifdef __ia64__
#include "syscalls-ia64.h"
#endif
#ifdef __sparc__
#include "syscalls-sparc.h"
#endif
#ifdef __s390x__
#include "syscalls-s390x.h"
#endif
#ifdef __s390__
#include "syscalls-s390.h"
#endif
#ifdef __arm__
#include "syscalls-arm.h"
#endif
#ifdef __mips__
#include "syscalls-mips.h"
#endif
#ifdef __sh__
#include "syscalls-sh.h"
#endif
#ifdef __alpha__
#include "syscalls-alpha.h"
#endif
#ifdef __aarch64__
#include "syscalls-aarch64.h"
#endif
#ifdef __hppa__
#include "syscalls-parisc.h"
#endif

#endif

#endif  /* _ARCH_SYSCALLS_H */
