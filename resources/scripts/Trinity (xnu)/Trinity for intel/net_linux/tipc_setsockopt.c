#include <stdlib.h>
#include <linux/tipc.h>
#include "net.h"
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_TIPC_OPTS ARRAY_SIZE(tipc_opts)
static const unsigned int tipc_opts[] = {
	TIPC_IMPORTANCE, TIPC_SRC_DROPPABLE, TIPC_DEST_DROPPABLE, TIPC_CONN_TIMEOUT,
	TIPC_NODE_RECVQ_DEPTH, TIPC_SOCK_RECVQ_DEPTH };

void tipc_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_TIPC;

	val = rand() % NR_SOL_TIPC_OPTS;
	so->optname = tipc_opts[val];

	so->optval = sizeof(__u32);
}
