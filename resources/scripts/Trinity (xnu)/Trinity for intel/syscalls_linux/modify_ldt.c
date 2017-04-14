#include "arch.h"

#ifdef X86
/*
 * asmlinkage int sys_modify_ldt(int func, void __user *ptr, unsigned long bytecount)
 */
#include <stdlib.h>
#include <sys/types.h>
#define __ASSEMBLY__ 1
#include <asm/ldt.h>
#include "sanitise.h"
#include "shm.h"

#define ALLOCSIZE LDT_ENTRIES * LDT_ENTRY_SIZE

static void sanitise_modify_ldt(int childno)
{
	void *ldt;
	//struct user_desc *desc;

	switch (shm->a1[childno]) {
	case 0:
		/* read the ldt into the memory pointed to by ptr.
		   The number of bytes read is the smaller of bytecount and the actual size of the ldt. */
		ldt = malloc(ALLOCSIZE);
		if (ldt == NULL)
			return;
		shm->a3[childno] = ALLOCSIZE;
		break;

	case 1:
		/* modify one ldt entry.
		 * ptr points to a user_desc structure
		 * bytecount must equal the size of this structure. */

	/*
               unsigned int  entry_number;
               unsigned long base_addr;
               unsigned int  limit;
               unsigned int  seg_32bit:1;
               unsigned int  contents:2;
               unsigned int  read_exec_only:1;
               unsigned int  limit_in_pages:1;
               unsigned int  seg_not_present:1;
               unsigned int  useable:1;
	*/
		break;
	default:
		break;
	}

	//FIXME: We leak 'ldt' here. Need to deallocate post-syscall.
}

struct syscall syscall_modify_ldt = {
	.name = "modify_ldt",
	.num_args = 3,
	.arg1name = "func",
	.arg1type = ARG_OP,
	.arg1list = {
		.num = 2,
		.values = { 0, 1 },
	},
	.arg2name = "ptr",
	.arg3name = "bytecount",
	.sanitise = sanitise_modify_ldt,
};
#endif
