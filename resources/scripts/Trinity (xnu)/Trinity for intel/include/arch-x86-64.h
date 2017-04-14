#define X86 1

#define PAGE_OFFSET	0xffff880000000000UL
#define KERNEL_ADDR	0xffffffff81000000UL
#define MODULE_ADDR	0xffffffffa0000000UL
#define VDSO_ADDR	0xffffffffff600000UL

#define TASK_SIZE       (0x800000000000UL - 4096)

#define PAGE_SHIFT 12

#define PTE_FILE_MAX_BITS 32

#define ARCH_IS_BIARCH 1
#define SYSCALLS32 syscalls_i386
#define SYSCALLS64 syscalls_x86_64
