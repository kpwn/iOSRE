#include <linux/ioctl.h>
#include <linux/types.h>

#include "trinity.h"
#include "ioctls.h"

/* drivers/staging/ozwpan/ozappif.h */

/* -----------------------------------------------------------------------------
 * Copyright (c) 2011 Ozmo Inc
 * Released under the GNU General Public License Version 2 (GPLv2).
 * -----------------------------------------------------------------------------
 */

#define OZ_IOCTL_MAGIC	0xf4

struct oz_mac_addr {
	__u8 a[6];
};

#define OZ_MAX_PDS	8

struct oz_pd_list {
	__u32 count;
	struct oz_mac_addr addr[OZ_MAX_PDS];
};

#define OZ_MAX_BINDING_LEN	32

struct oz_binding_info {
	char name[OZ_MAX_BINDING_LEN];
};

#define OZ_IOCTL_GET_PD_LIST	_IOR(OZ_IOCTL_MAGIC, 0, struct oz_pd_list)
#define OZ_IOCTL_SET_ACTIVE_PD	_IOW(OZ_IOCTL_MAGIC, 1, struct oz_mac_addr)
#define OZ_IOCTL_GET_ACTIVE_PD	_IOR(OZ_IOCTL_MAGIC, 2, struct oz_mac_addr)
#define OZ_IOCTL_ADD_BINDING	_IOW(OZ_IOCTL_MAGIC, 3, struct oz_binding_info)
#define OZ_IOCTL_REMOVE_BINDING	_IOW(OZ_IOCTL_MAGIC, 4, struct oz_binding_info)
#define OZ_IOCTL_MAX		5

static const struct ioctl ozwpan_ioctls[] = {
	IOCTL(OZ_IOCTL_GET_PD_LIST),
	IOCTL(OZ_IOCTL_SET_ACTIVE_PD),
	IOCTL(OZ_IOCTL_GET_ACTIVE_PD),
	IOCTL(OZ_IOCTL_ADD_BINDING),
	IOCTL(OZ_IOCTL_REMOVE_BINDING),
};

static const char *const ozwpan_devs[] = {
	"ozwpan",
};

static const struct ioctl_group ozwpan_grp = {
	.devtype = DEV_CHAR,
	.devs = ozwpan_devs,
	.devs_cnt = ARRAY_SIZE(ozwpan_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = ozwpan_ioctls,
	.ioctls_cnt = ARRAY_SIZE(ozwpan_ioctls),
};

REG_IOCTL_GROUP(ozwpan_grp)
