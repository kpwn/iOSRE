#include "config.h"

#ifdef USE_BTRFS

#include <stdlib.h>
#include <linux/btrfs.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl btrfs_control_ioctls[] = {
	IOCTL(BTRFS_IOC_SCAN_DEV),
	IOCTL(BTRFS_IOC_DEVICES_READY),
};

static const char *const btrfs_control_devs[] = {
	"btrfs-control",
};

static const struct ioctl_group btrfs_control_grp = {
	.devtype = DEV_MISC,
	.devs = btrfs_control_devs,
	.devs_cnt = ARRAY_SIZE(btrfs_control_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = btrfs_control_ioctls,
	.ioctls_cnt = ARRAY_SIZE(btrfs_control_ioctls),
};

REG_IOCTL_GROUP(btrfs_control_grp)

#endif
