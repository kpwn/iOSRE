#include <linux/ioctl.h>
#include <linux/fb.h>

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl fb_ioctls[] = {
	IOCTL(FBIOGET_VSCREENINFO),
	IOCTL(FBIOPUT_VSCREENINFO),
	IOCTL(FBIOGET_FSCREENINFO),
	IOCTL(FBIOGETCMAP),
	IOCTL(FBIOPUTCMAP),
	IOCTL(FBIOPAN_DISPLAY),
	IOCTL(FBIO_CURSOR),
	IOCTL(FBIOGET_CON2FBMAP),
	IOCTL(FBIOPUT_CON2FBMAP),
	IOCTL(FBIOBLANK),
	IOCTL(FBIOGET_VBLANK),
	IOCTL(FBIO_ALLOC),
	IOCTL(FBIO_FREE),
	IOCTL(FBIOGET_GLYPH),
	IOCTL(FBIOGET_HWCINFO),
	IOCTL(FBIOPUT_MODEINFO),
	IOCTL(FBIOGET_DISPINFO),
#ifdef FBIO_WAITFORVSYNC
	IOCTL(FBIO_WAITFORVSYNC),
#endif
};

static const char *const fb_chardevs[] = {
	"fb",
};

static const struct ioctl_group fb_grp = {
	.devtype = DEV_CHAR,
	.devs = fb_chardevs,
	.devs_cnt = ARRAY_SIZE(fb_chardevs),
	.sanitise = pick_random_ioctl,
	.ioctls = fb_ioctls,
	.ioctls_cnt = ARRAY_SIZE(fb_ioctls),
};

REG_IOCTL_GROUP(fb_grp)
