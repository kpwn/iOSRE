#include <linux/ioctl.h>
#include <video/sisfb.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl sisfb_ioctls[] = {
	IOCTL(SISFB_GET_INFO_SIZE),
	IOCTL(SISFB_GET_INFO),
	IOCTL(SISFB_GET_VBRSTATUS),
	IOCTL(SISFB_GET_AUTOMAXIMIZE),
	IOCTL(SISFB_SET_AUTOMAXIMIZE),
	IOCTL(SISFB_GET_TVPOSOFFSET),
	IOCTL(SISFB_SET_TVPOSOFFSET),
	IOCTL(SISFB_COMMAND),
	IOCTL(SISFB_SET_LOCK),
	IOCTL(SISFB_GET_INFO_OLD),
	IOCTL(SISFB_GET_VBRSTATUS_OLD),
	IOCTL(SISFB_GET_AUTOMAXIMIZE_OLD),
	IOCTL(SISFB_SET_AUTOMAXIMIZE_OLD),
};

static const char *const sisfb_devs[] = {
	"sisfb",
};

static const struct ioctl_group sisfb_grp = {
	.devtype = DEV_CHAR,
	.devs = sisfb_devs,
	.devs_cnt = ARRAY_SIZE(sisfb_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = sisfb_ioctls,
	.ioctls_cnt = ARRAY_SIZE(sisfb_ioctls),
};

REG_IOCTL_GROUP(sisfb_grp)
