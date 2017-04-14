#ifndef TRINITY_IOCTLS_H
#define TRINITY_IOCTLS_H

#include <stdlib.h>
#include <sys/stat.h>

struct ioctl {
	const char *name;
	unsigned int request;
};

#define DEV_CHAR  1
#define DEV_BLOCK 2
#define DEV_MISC  3

struct ioctl_group {
	/* optional user visible string that describes this group of ioctl
	 * operations. */
	const char *name;

	/* Non-NULL sanitise routine for this ioctl group. Initialize to
	 * "pick_random_ioctl" to pick one random ioctl. */
	void (*sanitise)(const struct ioctl_group *, int childno);

	/* Plug the available ioctls here. */
	const struct ioctl *ioctls;
	size_t ioctls_cnt;

	/* One of the DEV_* constants. */
	int devtype;

	/* List the device names from /proc/devices or /proc/misc that these
	 * ioctl operations are valid for. */
	const char *const *devs;
	size_t devs_cnt;

	/* Optional routine that should return 0 if the file descriptor is
	 * valid for this group. */
	int (*fd_test)(int fd, const struct stat *);
};

void register_ioctl_group(const struct ioctl_group *);

const struct ioctl_group *find_ioctl_group(int fd);

const struct ioctl_group *get_random_ioctl_group(void);

void pick_random_ioctl(const struct ioctl_group *, int childno);

void dump_ioctls(void);

#define IOCTL(_request) \
	{ .request = _request, .name = #_request, }

#define REG_IOCTL_GROUP(_struct) \
	static void __attribute__((constructor)) register_##_struct(void) { \
		register_ioctl_group(&_struct); \
	}

#endif /* TRINITY_IOCTLS_H */
