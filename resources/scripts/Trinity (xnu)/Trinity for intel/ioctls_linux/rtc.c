#include <linux/ioctl.h>
#include <linux/rtc.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl rtc_ioctls[] = {
	IOCTL(RTC_AIE_ON),
	IOCTL(RTC_AIE_OFF),
	IOCTL(RTC_UIE_ON),
	IOCTL(RTC_UIE_OFF),
	IOCTL(RTC_PIE_ON),
	IOCTL(RTC_PIE_OFF),
	IOCTL(RTC_WIE_ON),
	IOCTL(RTC_WIE_OFF),
	IOCTL(RTC_ALM_SET),
	IOCTL(RTC_ALM_READ),
	IOCTL(RTC_RD_TIME),
	IOCTL(RTC_SET_TIME),
	IOCTL(RTC_IRQP_READ),
	IOCTL(RTC_IRQP_SET),
	IOCTL(RTC_EPOCH_READ),
	IOCTL(RTC_EPOCH_SET),
	IOCTL(RTC_WKALM_SET),
	IOCTL(RTC_WKALM_RD),
	IOCTL(RTC_PLL_GET),
	IOCTL(RTC_PLL_SET),
#ifdef RTC_VL_READ
	IOCTL(RTC_VL_READ),
#endif
#ifdef RTC_VL_CLR
	IOCTL(RTC_VL_CLR),
#endif
};

static const char *const rtc_devs[] = {
	"rtc",
};

static const struct ioctl_group rtc_grp = {
	.devtype = DEV_CHAR,
	.devs = rtc_devs,
	.devs_cnt = ARRAY_SIZE(rtc_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = rtc_ioctls,
	.ioctls_cnt = ARRAY_SIZE(rtc_ioctls),
};

REG_IOCTL_GROUP(rtc_grp)
