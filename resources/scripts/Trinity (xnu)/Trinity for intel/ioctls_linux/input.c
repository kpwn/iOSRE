#include <linux/ioctl.h>
#include <linux/input.h>

#include "shm.h"
#include "trinity.h"
#include "ioctls.h"

static const struct ioctl input_ioctls[] = {
	IOCTL(EVIOCGVERSION),
	IOCTL(EVIOCGID),
	IOCTL(EVIOCGREP),
	IOCTL(EVIOCSREP),
	IOCTL(EVIOCGKEYCODE),
#ifdef EVIOCGKEYCODE_V2
	IOCTL(EVIOCGKEYCODE_V2),
#endif
	IOCTL(EVIOCSKEYCODE),
#ifdef EVIOCSKEYCODE_V2
	IOCTL(EVIOCSKEYCODE_V2),
#endif
	IOCTL(EVIOCGNAME(0)),
	IOCTL(EVIOCGPHYS(0)),
	IOCTL(EVIOCGUNIQ(0)),
#ifdef EVIOCGPROP
	IOCTL(EVIOCGPROP(0)),
#endif
#ifdef EVIOCGMTSLOTS
	IOCTL(EVIOCGMTSLOTS(0)),
#endif
	IOCTL(EVIOCGKEY(0)),
	IOCTL(EVIOCGLED(0)),
	IOCTL(EVIOCGSND(0)),
	IOCTL(EVIOCGSW(0)),
	IOCTL(EVIOCGBIT(0,0)),
	IOCTL(EVIOCGABS(0)),
	IOCTL(EVIOCSABS(0)),
	IOCTL(EVIOCSFF),
	IOCTL(EVIOCRMFF),
	IOCTL(EVIOCGEFFECTS),
	IOCTL(EVIOCGRAB),
#ifdef EVIOCSCLOCKID
	IOCTL(EVIOCSCLOCKID),
#endif
};

static const char *const input_devs[] = {
	"input",
};

static void input_sanitise(const struct ioctl_group *grp, int childno)
{
	unsigned int u, r;

	pick_random_ioctl(grp, childno);

	switch (shm->a2[childno]) {
	case EVIOCGNAME(0):
		u = rand();
		shm->a2[childno] = EVIOCGNAME(u);
		break;
	case EVIOCGPHYS(0):
		u = rand();
		shm->a2[childno] = EVIOCGPHYS(u);
		break;
	case EVIOCGUNIQ(0):
		u = rand();
		shm->a2[childno] = EVIOCGUNIQ(u);
		break;
#ifdef EVIOCGPROP
	case EVIOCGPROP(0):
		u = rand();
		shm->a2[childno] = EVIOCGPROP(u);
		break;
#endif
#ifdef EVIOCGMTSLOTS
	case EVIOCGMTSLOTS(0):
		u = rand();
		shm->a2[childno] = EVIOCGMTSLOTS(u);
		break;
#endif
	case EVIOCGKEY(0):
		u = rand();
		shm->a2[childno] = EVIOCGKEY(u);
		break;
	case EVIOCGLED(0):
		u = rand();
		shm->a2[childno] = EVIOCGLED(u);
		break;
	case EVIOCGSND(0):
		u = rand();
		shm->a2[childno] = EVIOCGSND(u);
		break;
	case EVIOCGSW(0):
		u = rand();
		shm->a2[childno] = EVIOCGSW(u);
		break;
	case EVIOCGBIT(0,0):
		u = rand();
		r = rand();
		if (u % 10) u %= EV_CNT;
		if (r % 10) r /= 4;
		shm->a2[childno] = EVIOCGBIT(u, r);
		break;
	case EVIOCGABS(0):
		u = rand();
		if (u % 10) u %= ABS_CNT;
		shm->a2[childno] = EVIOCGABS(u);
		break;
	case EVIOCSABS(0):
		u = rand();
		if (u % 10) u %= ABS_CNT;
		shm->a2[childno] = EVIOCSABS(u);
		break;
	default:
		break;
	}
}

static const struct ioctl_group input_grp = {
	.devtype = DEV_MISC,
	.devs = input_devs,
	.devs_cnt = ARRAY_SIZE(input_devs),
	.sanitise = input_sanitise,
	.ioctls = input_ioctls,
	.ioctls_cnt = ARRAY_SIZE(input_ioctls),
};

REG_IOCTL_GROUP(input_grp)
