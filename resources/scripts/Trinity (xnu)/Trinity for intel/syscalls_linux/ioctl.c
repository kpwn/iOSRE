/*
 * SYSCALL_DEFINE3(ioctl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
 */
#include <stdlib.h>
#include <linux/ioctl.h>
#include <linux/major.h>
#include "random.h"
#include "sanitise.h"
#include "maps.h"
#include "shm.h"
#include "ioctls.h"

static void ioctl_mangle_cmd(int childno)
{
	unsigned int i;

	/* mangle the cmd by ORing up to 4 random bits */
	for (i=0; i < (unsigned int)(rand() % 4); i++)
		shm->a2[childno] |= 1L << (rand() % 32);

	/* mangle the cmd by ANDing up to 4 random bits */
	for (i=0; i < (unsigned int)(rand() % 4); i++)
		shm->a2[childno] &= 1L << (rand() % 32);
}

static void ioctl_mangle_arg(int childno)
{
	/* the argument could mean anything, because ioctl sucks like that. */
	switch (rand_bool()) {
	case 0:	shm->a3[childno] = rand32();
		break;

	case 1:	shm->a3[childno] = (unsigned long) page_rand;
		generate_random_page(page_rand);
		break;
	default: break;
	}
}

static void generic_sanitise_ioctl(int childno)
{
	if ((rand() % 50)==0)
		ioctl_mangle_cmd(childno);

	ioctl_mangle_arg(childno);
}

static void sanitise_ioctl(int childno)
{
	const struct ioctl_group *grp;

	if (rand() % 100 == 0)
		grp = get_random_ioctl_group();
	else
		grp = find_ioctl_group(shm->a1[childno]);

	if (grp) {
		ioctl_mangle_arg(childno);

		grp->sanitise(grp, childno);

		if (rand() % 100 == 0)
			ioctl_mangle_cmd(childno);
	} else
		generic_sanitise_ioctl(childno);
}

struct syscall syscall_ioctl = {
	.name = "ioctl",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "cmd",
	.arg3name = "arg",
	.arg3type = ARG_RANDPAGE,
	.sanitise = sanitise_ioctl,
	.flags = NEED_ALARM,
};
