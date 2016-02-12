#include <linux/ioctl.h>
#include <linux/types.h>

#include "trinity.h"
#include "ioctls.h"

/* drivers/usb/mon/mon_bin.c */
/*
 * The USB Monitor, inspired by Dave Harding's USBMon.
 *
 * This is a binary format reader.
 *
 * Copyright (C) 2006 Paolo Abeni (paolo.abeni@email.it)
 * Copyright (C) 2006,2007 Pete Zaitcev (zaitcev@redhat.com)
 */

/* ioctl macros */
#define MON_IOC_MAGIC 0x92

#define MON_IOCQ_URB_LEN _IO(MON_IOC_MAGIC, 1)
/* #2 used to be MON_IOCX_URB, removed before it got into Linus tree */
#define MON_IOCG_STATS _IOR(MON_IOC_MAGIC, 3, struct mon_bin_stats)
#define MON_IOCT_RING_SIZE _IO(MON_IOC_MAGIC, 4)
#define MON_IOCQ_RING_SIZE _IO(MON_IOC_MAGIC, 5)
#define MON_IOCX_GET   _IOW(MON_IOC_MAGIC, 6, struct mon_bin_get)
#define MON_IOCX_MFETCH _IOWR(MON_IOC_MAGIC, 7, struct mon_bin_mfetch)
#define MON_IOCH_MFLUSH _IO(MON_IOC_MAGIC, 8)
/* #9 was MON_IOCT_SETAPI */
#define MON_IOCX_GETX   _IOW(MON_IOC_MAGIC, 10, struct mon_bin_get)

struct mon_bin_stats {
	__u32 queued;
	__u32 dropped;
};

struct mon_bin_get {
	/*struct mon_bin_hdr __user *hdr;*/	/* Can be 48 bytes or 64. */
	void *hdr;
	void /*__user*/ *data;
	size_t alloc;		/* Length of data (can be zero) */
};

struct mon_bin_mfetch {
	__u32 /*__user*/ *offvec;	/* Vector of events fetched */
	__u32 nfetch;		/* Number of events to fetch (out: fetched) */
	__u32 nflush;		/* Number of events to flush */
};

static const struct ioctl usbmon_ioctls[] = {
	IOCTL(MON_IOCQ_URB_LEN),
	IOCTL(MON_IOCG_STATS),
	IOCTL(MON_IOCT_RING_SIZE),
	IOCTL(MON_IOCQ_RING_SIZE),
	IOCTL(MON_IOCX_GET),
	IOCTL(MON_IOCX_MFETCH),
	IOCTL(MON_IOCH_MFLUSH),
	IOCTL(MON_IOCX_GETX),
};

static const char *const usbmon_devs[] = {
	"usbmon",
};

static const struct ioctl_group usbmon_grp = {
	.devtype = DEV_CHAR,
	.devs = usbmon_devs,
	.devs_cnt = ARRAY_SIZE(usbmon_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = usbmon_ioctls,
	.ioctls_cnt = ARRAY_SIZE(usbmon_ioctls),
};

REG_IOCTL_GROUP(usbmon_grp)
