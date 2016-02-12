#define X86 1

#define KERNEL_ADDR	0xc0100220
#define MODULE_ADDR     0xa0000000	// FIXME: Placeholder

#define PAGE_OFFSET 0xC0000000
#define TASK_SIZE (PAGE_OFFSET)
/*
 * Alternative possibilities for PAGE_OFFSET:
 * default 0xB0000000 if VMSPLIT_3G_OPT
 * default 0x78000000 if VMSPLIT_2G
 * default 0x40000000 if VMSPLIT_1G
 */

#define PAGE_SHIFT 12

#define PTE_FILE_MAX_BITS 31

#define SYSCALLS syscalls_i386
