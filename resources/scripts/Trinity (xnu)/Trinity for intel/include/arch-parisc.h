// FIXME: depends on kernel bit size, userspace is always 32 bit

#define PAGE_OFFSET 0x10000000

#if 0
// for 64 bit
#define PAGE_OFFSET 0x40000000
#endif

#define KERNEL_ADDR	0xa0000000	// FIXME: Placeholder
#define MODULE_ADDR     0xa0000000L	// FIXME: Placeholder
#define TASK_SIZE 0xa0000000	// FIXME: Placeholder

#define PAGE_SHIFT 		12
#define PTE_FILE_MAX_BITS	(32 - 11)

#define PTRACE_GETREGS		0
#define PTRACE_GETFPREGS	0
#define PTRACE_SETREGS		0
#define PTRACE_SETFPREGS	0

#define SYSCALLS syscalls_parisc
