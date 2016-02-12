#include <inttypes.h>
#include <stdlib.h>

/* drivers/staging/android/logger.h */

/* include/linux/logger.h
 *
 * Copyright (C) 2007-2008 Google, Inc.
 * Author: Robert Love <rlove@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/types.h>
#include <linux/ioctl.h>

/**
 * struct logger_entry - defines a single entry that is given to a logger
 * @len:	The length of the payload
 * @__pad:	Two bytes of padding that appear to be required
 * @pid:	The generating process' process ID
 * @tid:	The generating process' thread ID
 * @sec:	The number of seconds that have elapsed since the Epoch
 * @nsec:	The number of nanoseconds that have elapsed since @sec
 * @msg:	The message that is to be logged
 */
struct logger_entry {
	__u16		len;
	__u16		__pad;
	__s32		pid;
	__s32		tid;
	__s32		sec;
	__s32		nsec;
	char		msg[0];
};

#define LOGGER_LOG_RADIO	"log_radio"	/* radio-related messages */
#define LOGGER_LOG_EVENTS	"log_events"	/* system/hardware events */
#define LOGGER_LOG_SYSTEM	"log_system"	/* system/framework messages */
#define LOGGER_LOG_MAIN		"log_main"	/* everything else */

#define LOGGER_ENTRY_MAX_LEN		(4*1024)
#define LOGGER_ENTRY_MAX_PAYLOAD	\
	(LOGGER_ENTRY_MAX_LEN - sizeof(struct logger_entry))

#define __LOGGERIO	0xAE

#define LOGGER_GET_LOG_BUF_SIZE		_IO(__LOGGERIO, 1) /* size of log */
#define LOGGER_GET_LOG_LEN		_IO(__LOGGERIO, 2) /* used log len */
#define LOGGER_GET_NEXT_ENTRY_LEN	_IO(__LOGGERIO, 3) /* next entry len */
#define LOGGER_FLUSH_LOG		_IO(__LOGGERIO, 4) /* flush log */

#include "shm.h"
#include "trinity.h"
#include "ioctls.h"

static const struct ioctl logger_ioctls[] = {
	IOCTL(LOGGER_GET_LOG_BUF_SIZE),
	IOCTL(LOGGER_GET_LOG_LEN),
	IOCTL(LOGGER_GET_NEXT_ENTRY_LEN),
	IOCTL(LOGGER_FLUSH_LOG),
};

static const char *const logger_miscdevs[] = {
	LOGGER_LOG_RADIO,
	LOGGER_LOG_EVENTS,
	LOGGER_LOG_SYSTEM,
	LOGGER_LOG_MAIN,
};

static const struct ioctl_group logger_grp = {
	.devtype = DEV_MISC,
	.devs = logger_miscdevs,
	.devs_cnt = ARRAY_SIZE(logger_miscdevs),
	.sanitise = pick_random_ioctl,
	.ioctls = logger_ioctls,
	.ioctls_cnt = ARRAY_SIZE(logger_ioctls),
};

REG_IOCTL_GROUP(logger_grp)
