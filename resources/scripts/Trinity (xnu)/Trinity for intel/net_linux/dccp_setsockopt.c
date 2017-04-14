#include <stdlib.h>
#include <linux/dccp.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "config.h"
#include "trinity.h"	// ARRAY_SIZE

#define SOL_DCCP 269

#define NR_SOL_DCCP_OPTS ARRAY_SIZE(dccp_opts)
static const unsigned int dccp_opts[] = {
	DCCP_SOCKOPT_PACKET_SIZE, DCCP_SOCKOPT_SERVICE, DCCP_SOCKOPT_CHANGE_L, DCCP_SOCKOPT_CHANGE_R,
	DCCP_SOCKOPT_GET_CUR_MPS, DCCP_SOCKOPT_SERVER_TIMEWAIT, DCCP_SOCKOPT_SEND_CSCOV, DCCP_SOCKOPT_RECV_CSCOV,
	DCCP_SOCKOPT_AVAILABLE_CCIDS, DCCP_SOCKOPT_CCID, DCCP_SOCKOPT_TX_CCID, DCCP_SOCKOPT_RX_CCID,
	DCCP_SOCKOPT_QPOLICY_ID, DCCP_SOCKOPT_QPOLICY_TXQLEN, DCCP_SOCKOPT_CCID_RX_INFO, DCCP_SOCKOPT_CCID_TX_INFO };

void dccp_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_DCCP;

	val = rand() % NR_SOL_DCCP_OPTS;
	so->optname = dccp_opts[val];
}
