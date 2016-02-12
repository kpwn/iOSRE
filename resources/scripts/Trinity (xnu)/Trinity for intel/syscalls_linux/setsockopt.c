/*
 * SYSCALL_DEFINE5(setsockopt, int, fd, int, level, int, optname, char __user *, optval, int, optlen)
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/types.h>
#include "sanitise.h"
#include "compat.h"
#include "maps.h"
#include "shm.h"
#include "net.h"
#include "config.h"
#include "random.h"
#include "trinity.h"

struct sso_funcptr {
	void (*func)(struct sockopt *so);
};

static const struct sso_funcptr ssoptrs[] = {
	{ .func = &ip_setsockopt },
	{ .func = &socket_setsockopt },
	{ .func = &tcp_setsockopt },
	{ .func = &udp_setsockopt },
	{ .func = &inet6_setsockopt },
	{ .func = &icmpv6_setsockopt },
	{ .func = &sctp_setsockopt },
	{ .func = &udplite_setsockopt },
	{ .func = &raw_setsockopt },
	{ .func = &ipx_setsockopt },
	{ .func = &ax25_setsockopt },
	{ .func = &atalk_setsockopt },
	{ .func = &netrom_setsockopt },
	{ .func = &rose_setsockopt },
	{ .func = &decnet_setsockopt },
	{ .func = &x25_setsockopt },
	{ .func = &packet_setsockopt },
	{ .func = &atm_setsockopt },
	{ .func = &aal_setsockopt },
	{ .func = &irda_setsockopt },
	{ .func = &netbeui_setsockopt },
	{ .func = &llc_setsockopt },
	{ .func = &dccp_setsockopt },
	{ .func = &netlink_setsockopt },
	{ .func = &tipc_setsockopt },
	{ .func = &rxrpc_setsockopt },
	{ .func = &pppol2tp_setsockopt },
	{ .func = &bluetooth_setsockopt },
	{ .func = &pnpipe_setsockopt },
	{ .func = &rds_setsockopt },
	{ .func = &iucv_setsockopt },
	{ .func = &caif_setsockopt },
	{ .func = &alg_setsockopt },
	{ .func = &nfc_setsockopt },
};

void do_setsockopt(struct sockopt *so)
{
	so->optval = (unsigned long) page_rand;
	// pick a size for optlen. At the minimum, we want an int (overridden below)
	if (rand_bool())
		so->optlen = sizeof(int);
	else
		so->optlen = rand() % 256;

	if (rand() % 100 > 0) {
		ssoptrs[rand() % ARRAY_SIZE(ssoptrs)].func(so);
	} else {
		so->level = rand();
		so->optname = (rand() % 0x100);	/* random operation. */
	}

	/*
	 * 10% of the time, mangle the options.
	 * This should catch new options we don't know about, and also maybe some missing bounds checks.
	 */
	if ((rand() % 100) < 10)
		so->optname |= (1 << (rand() % 32));


	/* optval should be nonzero to enable a boolean option, or zero if the option is to be disabled.
	 * Let's disable it half the time.
	 */
	if (rand_bool())
		so->optval = 0;

}

static void sanitise_setsockopt(int childno)
{
	struct sockopt so = { 0, 0, 0, 0 };

	do_setsockopt(&so);

	/* copy the generated values to the shm. */
	shm->a2[childno] = so.level;
	shm->a3[childno] = so.optname;
	shm->a4[childno] = so.optval;
	shm->a5[childno] = so.optlen;
}

struct syscall syscall_setsockopt = {
	.name = "setsockopt",
	.num_args = 5,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "level",
	.arg3name = "optname",
	.arg4name = "optval",
	.arg4type = ARG_ADDRESS,
	.arg5name = "optlen",
	.sanitise = sanitise_setsockopt,
	.flags = NEED_ALARM,
};
