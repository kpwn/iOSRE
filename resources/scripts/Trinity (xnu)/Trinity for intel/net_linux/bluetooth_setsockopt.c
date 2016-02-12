#include <stdlib.h>
#include "net.h"
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_BLUETOOTH 274

#define NR_SOL_BLUETOOTH_OPTS ARRAY_SIZE(bluetooth_opts)
static const unsigned int bluetooth_opts[] = {
	BT_SECURITY, BT_DEFER_SETUP, BT_FLUSHABLE, BT_POWER,
	BT_CHANNEL_POLICY };

#define NR_SOL_BLUETOOTH_HCI_OPTS ARRAY_SIZE(bluetooth_hci_opts)
static const unsigned int bluetooth_hci_opts[] = {
	HCI_DATA_DIR, HCI_FILTER, HCI_TIME_STAMP };

#define NR_SOL_BLUETOOTH_L2CAP_OPTS ARRAY_SIZE(bluetooth_l2cap_opts)
static const unsigned int bluetooth_l2cap_opts[] = {
	L2CAP_OPTIONS, L2CAP_LM };

#define NR_SOL_BLUETOOTH_RFCOMM_OPTS ARRAY_SIZE(bluetooth_rfcomm_opts)
static const unsigned int bluetooth_rfcomm_opts[] = { RFCOMM_LM };


void bluetooth_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_BLUETOOTH;

	switch(rand() % 5) {
	case 0: so->level = SOL_HCI; break;
	case 1: so->level = SOL_L2CAP; break;
	case 2: so->level = SOL_SCO; break;
	case 3: so->level = SOL_RFCOMM; break;
	case 4: /* leave level unchanged */
		;;
	default:
		break;
	}

	switch (so->level) {
	case SOL_HCI:
		val = rand() % NR_SOL_BLUETOOTH_HCI_OPTS;
		so->optname = bluetooth_hci_opts[val];
		break;

	case SOL_L2CAP:
		val = rand() % NR_SOL_BLUETOOTH_L2CAP_OPTS;
		so->optname = bluetooth_l2cap_opts[val];
		break;

	case SOL_SCO:   /* no options currently */
		break;

	case SOL_RFCOMM:
		val = rand() % NR_SOL_BLUETOOTH_RFCOMM_OPTS;
		so->optname = bluetooth_rfcomm_opts[val];
		break;

	case SOL_BLUETOOTH:
		val = rand() % NR_SOL_BLUETOOTH_OPTS;
		so->optname = bluetooth_opts[val];
		break;

	default: break;
	}
}
