#include <linux/ioctl.h>
#include <linux/uinput.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl uinput_ioctls[] = {
	IOCTL(UI_DEV_CREATE),
	IOCTL(UI_DEV_DESTROY),
	IOCTL(UI_SET_EVBIT),
	IOCTL(UI_SET_KEYBIT),
	IOCTL(UI_SET_RELBIT),
	IOCTL(UI_SET_ABSBIT),
	IOCTL(UI_SET_MSCBIT),
	IOCTL(UI_SET_LEDBIT),
	IOCTL(UI_SET_SNDBIT),
	IOCTL(UI_SET_FFBIT),
	IOCTL(UI_SET_PHYS),
	IOCTL(UI_SET_SWBIT),
#ifdef UI_SET_PROPBIT
	IOCTL(UI_SET_PROPBIT),
#endif
	IOCTL(UI_BEGIN_FF_UPLOAD),
	IOCTL(UI_END_FF_UPLOAD),
	IOCTL(UI_BEGIN_FF_ERASE),
	IOCTL(UI_END_FF_ERASE),
};

static const char *const uinput_devs[] = {
	"uinput",
};

static const struct ioctl_group uinput_grp = {
	.devtype = DEV_MISC,
	.devs = uinput_devs,
	.devs_cnt = ARRAY_SIZE(uinput_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = uinput_ioctls,
	.ioctls_cnt = ARRAY_SIZE(uinput_ioctls),
};

REG_IOCTL_GROUP(uinput_grp)
