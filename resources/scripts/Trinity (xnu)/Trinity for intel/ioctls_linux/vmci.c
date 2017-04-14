#include <linux/ioctl.h>

/* include/linux/vmw_vmci_defs.h */

/*
 * VMware VMCI Driver
 *
 * Copyright (C) 2012 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

/* --- */

/*
 * The VMCI IOCTLs.  We use identity code 7, as noted in ioctl-number.h, and
 * we start at sequence 9f.  This gives us the same values that our shipping
 * products use, starting at 1951, provided we leave out the direction and
 * structure size.  Note that VMMon occupies the block following us, starting
 * at 2001.
 */
#define IOCTL_VMCI_VERSION			_IO(7, 0x9f)	/* 1951 */
#define IOCTL_VMCI_INIT_CONTEXT			_IO(7, 0xa0)
#define IOCTL_VMCI_QUEUEPAIR_SETVA		_IO(7, 0xa4)
#define IOCTL_VMCI_NOTIFY_RESOURCE		_IO(7, 0xa5)
#define IOCTL_VMCI_NOTIFICATIONS_RECEIVE	_IO(7, 0xa6)
#define IOCTL_VMCI_VERSION2			_IO(7, 0xa7)
#define IOCTL_VMCI_QUEUEPAIR_ALLOC		_IO(7, 0xa8)
#define IOCTL_VMCI_QUEUEPAIR_SETPAGEFILE	_IO(7, 0xa9)
#define IOCTL_VMCI_QUEUEPAIR_DETACH		_IO(7, 0xaa)
#define IOCTL_VMCI_DATAGRAM_SEND		_IO(7, 0xab)
#define IOCTL_VMCI_DATAGRAM_RECEIVE		_IO(7, 0xac)
#define IOCTL_VMCI_CTX_ADD_NOTIFICATION		_IO(7, 0xaf)
#define IOCTL_VMCI_CTX_REMOVE_NOTIFICATION	_IO(7, 0xb0)
#define IOCTL_VMCI_CTX_GET_CPT_STATE		_IO(7, 0xb1)
#define IOCTL_VMCI_CTX_SET_CPT_STATE		_IO(7, 0xb2)
#define IOCTL_VMCI_GET_CONTEXT_ID		_IO(7, 0xb3)
#define IOCTL_VMCI_SOCKETS_VERSION		_IO(7, 0xb4)
#define IOCTL_VMCI_SOCKETS_GET_AF_VALUE		_IO(7, 0xb8)
#define IOCTL_VMCI_SOCKETS_GET_LOCAL_CID	_IO(7, 0xb9)
#define IOCTL_VMCI_SET_NOTIFY			_IO(7, 0xcb)	/* 1995 */
/*IOCTL_VMMON_START				_IO(7, 0xd1)*/	/* 2001 */

#include "trinity.h"
#include "ioctls.h"

static const struct ioctl vmci_ioctls[] = {
	IOCTL(IOCTL_VMCI_VERSION),
	IOCTL(IOCTL_VMCI_INIT_CONTEXT),
	IOCTL(IOCTL_VMCI_QUEUEPAIR_SETVA),
	IOCTL(IOCTL_VMCI_NOTIFY_RESOURCE),
	IOCTL(IOCTL_VMCI_NOTIFICATIONS_RECEIVE),
	IOCTL(IOCTL_VMCI_VERSION2),
	IOCTL(IOCTL_VMCI_QUEUEPAIR_ALLOC),
	IOCTL(IOCTL_VMCI_QUEUEPAIR_SETPAGEFILE),
	IOCTL(IOCTL_VMCI_QUEUEPAIR_DETACH),
	IOCTL(IOCTL_VMCI_DATAGRAM_SEND),
	IOCTL(IOCTL_VMCI_DATAGRAM_RECEIVE),
	IOCTL(IOCTL_VMCI_CTX_ADD_NOTIFICATION),
	IOCTL(IOCTL_VMCI_CTX_REMOVE_NOTIFICATION),
	IOCTL(IOCTL_VMCI_CTX_GET_CPT_STATE),
	IOCTL(IOCTL_VMCI_CTX_SET_CPT_STATE),
	IOCTL(IOCTL_VMCI_GET_CONTEXT_ID),
	IOCTL(IOCTL_VMCI_SOCKETS_VERSION),
	IOCTL(IOCTL_VMCI_SOCKETS_GET_AF_VALUE),
	IOCTL(IOCTL_VMCI_SOCKETS_GET_LOCAL_CID),
	IOCTL(IOCTL_VMCI_SET_NOTIFY),
};

static const char *const vmci_devs[] = {
	"vmci",
};

static const struct ioctl_group vmci_grp = {
	.devtype = DEV_MISC,
	.devs = vmci_devs,
	.devs_cnt = ARRAY_SIZE(vmci_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = vmci_ioctls,
	.ioctls_cnt = ARRAY_SIZE(vmci_ioctls),
};

REG_IOCTL_GROUP(vmci_grp)
