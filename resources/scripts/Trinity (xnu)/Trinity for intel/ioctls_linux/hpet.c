#include <linux/ioctl.h>
#include <linux/hpet.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl hpet_ioctls[] = {
	IOCTL(HPET_IE_ON),
	IOCTL(HPET_IE_OFF),
	IOCTL(HPET_INFO),
	IOCTL(HPET_EPI),
	IOCTL(HPET_DPI),
	IOCTL(HPET_IRQFREQ),
};

static const char *const hpet_devs[] = {
	"hpet",
};

static const struct ioctl_group hpet_grp = {
	.devtype = DEV_MISC,
	.devs = hpet_devs,
	.devs_cnt = ARRAY_SIZE(hpet_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = hpet_ioctls,
	.ioctls_cnt = ARRAY_SIZE(hpet_ioctls),
};

REG_IOCTL_GROUP(hpet_grp)
