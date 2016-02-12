#include <linux/watchdog.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl watchdog_ioctls[] = {
	IOCTL(WDIOC_GETSUPPORT),
	IOCTL(WDIOC_GETSTATUS),
	IOCTL(WDIOC_GETBOOTSTATUS),
	IOCTL(WDIOC_GETTEMP),
	IOCTL(WDIOC_SETOPTIONS),
	IOCTL(WDIOC_KEEPALIVE),
	IOCTL(WDIOC_SETTIMEOUT),
	IOCTL(WDIOC_GETTIMEOUT),
	IOCTL(WDIOC_SETPRETIMEOUT),
	IOCTL(WDIOC_GETPRETIMEOUT),
	IOCTL(WDIOC_GETTIMELEFT),
};

static const char *const watchdog_devs[] = {
	"watchdog",
};

static const struct ioctl_group watchdog_grp_misc = {
	.devtype = DEV_MISC,
	.devs = watchdog_devs,
	.devs_cnt = ARRAY_SIZE(watchdog_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = watchdog_ioctls,
	.ioctls_cnt = ARRAY_SIZE(watchdog_ioctls),
};

REG_IOCTL_GROUP(watchdog_grp_misc)

static const struct ioctl_group watchdog_grp_char = {
	.devtype = DEV_CHAR,
	.devs = watchdog_devs,
	.devs_cnt = ARRAY_SIZE(watchdog_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = watchdog_ioctls,
	.ioctls_cnt = ARRAY_SIZE(watchdog_ioctls),
};

REG_IOCTL_GROUP(watchdog_grp_char)
