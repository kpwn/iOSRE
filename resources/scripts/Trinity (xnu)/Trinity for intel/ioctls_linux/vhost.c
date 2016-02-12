#include "config.h"

#ifdef USE_VHOST
#include <linux/vhost.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl vhost_ioctls[] = {
	IOCTL(VHOST_GET_FEATURES),
	IOCTL(VHOST_SET_FEATURES),
	IOCTL(VHOST_SET_OWNER),
	IOCTL(VHOST_RESET_OWNER),
	IOCTL(VHOST_SET_MEM_TABLE),
	IOCTL(VHOST_SET_LOG_BASE),
	IOCTL(VHOST_SET_LOG_FD),
	IOCTL(VHOST_SET_VRING_NUM),
	IOCTL(VHOST_SET_VRING_ADDR),
	IOCTL(VHOST_SET_VRING_BASE),
	IOCTL(VHOST_GET_VRING_BASE),
	IOCTL(VHOST_SET_VRING_KICK),
	IOCTL(VHOST_SET_VRING_CALL),
	IOCTL(VHOST_SET_VRING_ERR),
	IOCTL(VHOST_NET_SET_BACKEND),
#ifdef VHOST_SCSI_SET_ENDPOINT
	IOCTL(VHOST_SCSI_SET_ENDPOINT),
#endif
#ifdef VHOST_SCSI_CLEAR_ENDPOINT
	IOCTL(VHOST_SCSI_CLEAR_ENDPOINT),
#endif
#ifdef VHOST_SCSI_GET_ABI_VERSION
	IOCTL(VHOST_SCSI_GET_ABI_VERSION),
#endif
#ifdef VHOST_SCSI_SET_EVENTS_MISSED
	IOCTL(VHOST_SCSI_SET_EVENTS_MISSED),
#endif
#ifdef VHOST_SCSI_GET_EVENTS_MISSED
	IOCTL(VHOST_SCSI_GET_EVENTS_MISSED),
#endif
};

static const char *const vhost_devs[] = {
	"vhost-net",
};

static const struct ioctl_group vhost_grp = {
	.devtype = DEV_MISC,
	.devs = vhost_devs,
	.devs_cnt = ARRAY_SIZE(vhost_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = vhost_ioctls,
	.ioctls_cnt = ARRAY_SIZE(vhost_ioctls),
};

REG_IOCTL_GROUP(vhost_grp)
#endif /* USE_VHOST */
