#include <linux/ioctl.h>

#define IOCTL_VM_SOCKETS_GET_LOCAL_CID		_IO(7, 0xb9)

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl vsock_ioctls[] = {
	IOCTL(IOCTL_VM_SOCKETS_GET_LOCAL_CID),
};

static const char *const vsock_devs[] = {
	"vsock",
};

static const struct ioctl_group vsock_grp = {
	.devtype = DEV_MISC,
	.devs = vsock_devs,
	.devs_cnt = ARRAY_SIZE(vsock_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = vsock_ioctls,
	.ioctls_cnt = ARRAY_SIZE(vsock_ioctls),
};

REG_IOCTL_GROUP(vsock_grp)
