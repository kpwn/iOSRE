#include <stdlib.h>
#include <bits/sockaddr.h>
#include <linux/ax25.h>
#include <netrose/rose.h>
#include "net.h"
#include "maps.h"	// page_rand
#include "compat.h"
#include "trinity.h"	// ARRAY_SIZE

#define NR_SOL_ROSE_OPTS ARRAY_SIZE(rose_opts)
static const unsigned int rose_opts[] = {
	ROSE_DEFER, ROSE_T1, ROSE_T2, ROSE_T3,
	ROSE_IDLE, ROSE_QBITINCL, ROSE_HOLDBACK };

void rose_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_ROSE;

	val = rand() % NR_SOL_ROSE_OPTS;
	so->optname = rose_opts[val];
}
