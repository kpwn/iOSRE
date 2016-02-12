#include <linux/ioctl.h>
#include <linux/types.h>

#define X86_IOC_RDMSR_REGS	_IOWR('c', 0xA0, __u32[8])
#define X86_IOC_WRMSR_REGS	_IOWR('c', 0xA1, __u32[8])

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl msr_ioctls[] = {
	IOCTL(X86_IOC_RDMSR_REGS),
	//IOCTL(X86_IOC_WRMSR_REGS),	// Too dangerous.
};

static const char *const msr_devs[] = {
	"cpu/msr",
};

static const struct ioctl_group msr_grp = {
	.devtype = DEV_CHAR,
	.devs = msr_devs,
	.devs_cnt = ARRAY_SIZE(msr_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = msr_ioctls,
	.ioctls_cnt = ARRAY_SIZE(msr_ioctls),
};

REG_IOCTL_GROUP(msr_grp)
