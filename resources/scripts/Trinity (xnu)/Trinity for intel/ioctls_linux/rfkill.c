#include <linux/ioctl.h>
#include <linux/rfkill.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl rfkill_ioctls[] = {
	IOCTL(RFKILL_IOCTL_NOINPUT),
};

static const char *const rfkill_devs[] = {
	"rfkill",
};

static const struct ioctl_group rfkill_grp = {
	.devtype = DEV_MISC,
	.devs = rfkill_devs,
	.devs_cnt = ARRAY_SIZE(rfkill_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = rfkill_ioctls,
	.ioctls_cnt = ARRAY_SIZE(rfkill_ioctls),
};

REG_IOCTL_GROUP(rfkill_grp)
