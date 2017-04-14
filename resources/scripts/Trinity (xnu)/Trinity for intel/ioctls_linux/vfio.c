#include "config.h"

#ifdef USE_VFIO
#include <linux/vfio.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl vfio_ioctls[] = {
	IOCTL(VFIO_GET_API_VERSION),
	IOCTL(VFIO_CHECK_EXTENSION),
	IOCTL(VFIO_SET_IOMMU),
	IOCTL(VFIO_GROUP_GET_STATUS),
	IOCTL(VFIO_GROUP_SET_CONTAINER),
	IOCTL(VFIO_GROUP_UNSET_CONTAINER),
	IOCTL(VFIO_GROUP_GET_DEVICE_FD),
	IOCTL(VFIO_DEVICE_GET_INFO),
	IOCTL(VFIO_DEVICE_GET_REGION_INFO),
	IOCTL(VFIO_DEVICE_GET_IRQ_INFO),
	IOCTL(VFIO_DEVICE_SET_IRQS),
	IOCTL(VFIO_DEVICE_RESET),
	IOCTL(VFIO_IOMMU_GET_INFO),
	IOCTL(VFIO_IOMMU_MAP_DMA),
	IOCTL(VFIO_IOMMU_UNMAP_DMA),
};

static const char *const vfio_devs[] = {
	"vfio",
};

static const struct ioctl_group vfio_grp = {
	.devtype = DEV_CHAR,
	.devs = vfio_devs,
	.devs_cnt = ARRAY_SIZE(vfio_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = vfio_ioctls,
	.ioctls_cnt = ARRAY_SIZE(vfio_ioctls),
};

REG_IOCTL_GROUP(vfio_grp)
#endif
