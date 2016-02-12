#define KERNEL_ADDR	0xa000000100000000
#define MODULE_ADDR     0xffffffffa0000000L	// FIXME: Placeholder
#define PAGE_OFFSET 0xe000000000000000
#define TASK_SIZE 0xa000000000000000

#define PAGE_SHIFT 		12
#define PTE_FILE_MAX_BITS	61

#define SYSCALL_OFFSET	1024

// for __IA64_UL_CONST we need to include:
#include <asm/types.h>

#define PTRACE_GETFPREGS	0
#define PTRACE_SETFPREGS	0

#define SYSCALLS syscalls_ia64
