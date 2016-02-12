#include <linux/ioctl.h>
#include <mtd/mtd-abi.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl mtd_ioctls[] = {
	IOCTL(MEMGETINFO),
	IOCTL(MEMERASE),
	IOCTL(MEMWRITEOOB),
	IOCTL(MEMREADOOB),
	IOCTL(MEMLOCK),
	IOCTL(MEMUNLOCK),
	IOCTL(MEMGETREGIONCOUNT),
	IOCTL(MEMGETREGIONINFO),
	/* IOCTL(MEMSETOOBSEL), */
	IOCTL(MEMGETOOBSEL),
	IOCTL(MEMGETBADBLOCK),
	IOCTL(MEMSETBADBLOCK),
	IOCTL(OTPSELECT),
	IOCTL(OTPGETREGIONCOUNT),
	IOCTL(OTPGETREGIONINFO),
	IOCTL(OTPLOCK),
	IOCTL(ECCGETLAYOUT),
	IOCTL(ECCGETSTATS),
	IOCTL(MTDFILEMODE),
	IOCTL(MEMERASE64),
	IOCTL(MEMWRITEOOB64),
	IOCTL(MEMREADOOB64),
#ifdef MEMISLOCKED
	IOCTL(MEMISLOCKED),
#endif
#ifdef MEMWRITE
	IOCTL(MEMWRITE),
#endif
};

static const char *const mtd_devs[] = {
	"mtd",
};

static const struct ioctl_group mtd_grp = {
	.devtype = DEV_MISC,
	.devs = mtd_devs,
	.devs_cnt = ARRAY_SIZE(mtd_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = mtd_ioctls,
	.ioctls_cnt = ARRAY_SIZE(mtd_ioctls),
};

REG_IOCTL_GROUP(mtd_grp)
