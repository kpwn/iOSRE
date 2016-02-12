#include <linux/ioctl.h>

#include "trinity.h"
#include "ioctls.h"

#define MCE_GET_RECORD_LEN   _IOR('M', 1, int)
#define MCE_GET_LOG_LEN      _IOR('M', 2, int)
#define MCE_GETCLEAR_FLAGS   _IOR('M', 3, int)

static const struct ioctl mce_ioctls[] = {
	IOCTL(MCE_GET_RECORD_LEN),
	IOCTL(MCE_GET_LOG_LEN),
	IOCTL(MCE_GETCLEAR_FLAGS),
};

static const char *const mce_devs[] = {
	"mcelog",
};

static const struct ioctl_group mce_grp = {
	.devtype = DEV_MISC,
	.devs = mce_devs,
	.devs_cnt = ARRAY_SIZE(mce_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = mce_ioctls,
	.ioctls_cnt = ARRAY_SIZE(mce_ioctls),
};

REG_IOCTL_GROUP(mce_grp)
