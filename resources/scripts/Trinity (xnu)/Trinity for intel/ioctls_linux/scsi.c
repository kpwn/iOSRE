#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <scsi/scsi.h>
#include <scsi/scsi_ioctl.h>
#include <scsi/sg.h>

#include "shm.h"
#include "maps.h"
#include "trinity.h"	// ARRAY_SIZE
#include "arch.h"	// page_size
#include "random.h"
#include "sanitise.h"
#include "ioctls.h"

#ifndef SCSI_IOCTL_GET_PCI
#define SCSI_IOCTL_GET_PCI 0x5387
#endif

#ifndef SG_GET_ACCESS_COUNT
#define SG_GET_ACCESS_COUNT 0x2289
#endif

static const struct ioctl scsi_ioctls[] = {
	IOCTL(SCSI_IOCTL_GET_IDLUN),
	IOCTL(SCSI_IOCTL_TAGGED_ENABLE),
	IOCTL(SCSI_IOCTL_TAGGED_DISABLE),
	IOCTL(SCSI_IOCTL_PROBE_HOST),
	IOCTL(SCSI_IOCTL_GET_BUS_NUMBER),
	IOCTL(SCSI_IOCTL_GET_PCI),
	IOCTL(SCSI_IOCTL_SEND_COMMAND),
	IOCTL(SCSI_IOCTL_TEST_UNIT_READY),
	IOCTL(SCSI_IOCTL_BENCHMARK_COMMAND),
	IOCTL(SCSI_IOCTL_SYNC),
	IOCTL(SCSI_IOCTL_START_UNIT),
	IOCTL(SCSI_IOCTL_STOP_UNIT),
	IOCTL(SCSI_IOCTL_DOORLOCK),
	IOCTL(SCSI_IOCTL_DOORUNLOCK),

	IOCTL(SG_EMULATED_HOST),
	IOCTL(SG_SET_TRANSFORM),
	IOCTL(SG_GET_TRANSFORM),
	IOCTL(SG_SET_RESERVED_SIZE),
	IOCTL(SG_GET_RESERVED_SIZE),
	IOCTL(SG_GET_SCSI_ID),
	IOCTL(SG_SET_FORCE_LOW_DMA),
	IOCTL(SG_GET_LOW_DMA),
	IOCTL(SG_SET_FORCE_PACK_ID),
	IOCTL(SG_GET_PACK_ID),
	IOCTL(SG_GET_NUM_WAITING),
	IOCTL(SG_GET_SG_TABLESIZE),
	IOCTL(SG_GET_VERSION_NUM),
	IOCTL(SG_SCSI_RESET),
	IOCTL(SG_IO),
	IOCTL(SG_GET_REQUEST_TABLE),
	IOCTL(SG_SET_KEEP_ORPHAN),
	IOCTL(SG_GET_KEEP_ORPHAN),
	IOCTL(SG_GET_ACCESS_COUNT),
	IOCTL(SG_SET_TIMEOUT),
	IOCTL(SG_GET_TIMEOUT),
	IOCTL(SG_GET_COMMAND_Q),
	IOCTL(SG_SET_COMMAND_Q),
	IOCTL(SG_SET_DEBUG),
	IOCTL(SG_NEXT_CMD_LEN),
};

static const char *const scsi_devs[] = {
	"sd",
	"sr",
};

struct sgio {
	sg_io_hdr_t ioh;
	unsigned char data[512];
	unsigned char cmd[12];
	unsigned char sense[252];
};

static void scsi_sg_io_sanitise(int childno)
{
	struct sgio *sgio;

	sgio = (struct sgio *) page_rand;

	sgio->cmd[0] = 0x12;
	sgio->cmd[3] = 0x2;

	sgio->ioh.interface_id = 'S';

	switch (rand() % 4) {
	case 0:	sgio->ioh.dxfer_direction = SG_DXFER_NONE;	break;
	case 1:	sgio->ioh.dxfer_direction = SG_DXFER_TO_DEV;	break;
	case 2:	sgio->ioh.dxfer_direction = SG_DXFER_FROM_DEV;	break;
	case 3:	sgio->ioh.dxfer_direction = SG_DXFER_TO_FROM_DEV;	break;
	default: break;
	}

	sgio->ioh.dxferp = sgio->data;

	switch (rand() % 3) {
	case 0: sgio->ioh.dxfer_len = rand() % page_size;	break;
	case 1: sgio->ioh.dxfer_len = (unsigned int) rand32();	break;
	case 2: sgio->ioh.dxfer_len = rand() % 512;		break;
	default: break;
	}

	sgio->ioh.cmdp = sgio->cmd;
	sgio->ioh.cmd_len = 6;
	sgio->ioh.mx_sb_len = sizeof(sgio->sense);
	sgio->ioh.sbp = sgio->sense;
	sgio->ioh.timeout = UINT_MAX;
	sgio->ioh.usr_ptr = NULL;
	sgio->ioh.flags |= SG_FLAG_DIRECT_IO;

	shm->a3[childno] = (unsigned long) page_rand;
}

static void scsi_sanitise(const struct ioctl_group *grp, int childno)
{
	pick_random_ioctl(grp, childno);

	switch (shm->a2[childno]) {
	case SG_IO:
		scsi_sg_io_sanitise(childno);
		break;
	default:
		break;
	}
}

static const struct ioctl_group scsi_grp = {
	.devtype = DEV_BLOCK,
	.devs = scsi_devs,
	.devs_cnt = ARRAY_SIZE(scsi_devs),
	.sanitise = scsi_sanitise,
	.ioctls = scsi_ioctls,
	.ioctls_cnt = ARRAY_SIZE(scsi_ioctls),
};

REG_IOCTL_GROUP(scsi_grp)
