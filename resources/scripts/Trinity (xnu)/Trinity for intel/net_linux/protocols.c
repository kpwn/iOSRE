#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "trinity.h"
#include "constants.h"
#include "protocols.h"
#include "params.h"
#include "net.h"
#include "log.h"

struct protocol {
	const char *name;
	const unsigned int proto;
};

static const struct protocol protocols[] = {
	{ "PF_UNSPEC",       0 },
	{ "PF_LOCAL",        1 },
	{ "PF_UNIX",         PF_LOCAL },
	{ "PF_FILE",         PF_LOCAL },
	{ "PF_INET",         2 },
	{ "PF_AX25",         3 },
	{ "PF_IPX",          4 },
	{ "PF_APPLETALK",    5 },
	{ "PF_NETROM",       6 },
	{ "PF_BRIDGE",       7 },
	{ "PF_ATMPVC",       8 },
	{ "PF_X25",          9 },
	{ "PF_INET6",        10 },
	{ "PF_ROSE",         11 },
	{ "PF_DECnet",       12 },
	{ "PF_NETBEUI",      13 },
	{ "PF_SECURITY",     14 },
	{ "PF_KEY",          15 },
	{ "PF_NETLINK",      16 },
	{ "PF_ROUTE",        PF_NETLINK },
	{ "PF_PACKET",       17 },
	{ "PF_ASH",          18 },
	{ "PF_ECONET",       19 },
	{ "PF_ATMSVC",       20 },
	{ "PF_RDS",          21 },
	{ "PF_SNA",          22 },
	{ "PF_IRDA",         23 },
	{ "PF_PPPOX",        24 },
	{ "PF_WANPIPE",      25 },
	{ "PF_LLC",          26 },
	{ "PF_CAN",          29 },
	{ "PF_TIPC",         30 },
	{ "PF_BLUETOOTH",    31 },
	{ "PF_IUCV",         32 },
	{ "PF_RXRPC",        33 },
	{ "PF_ISDN",         34 },
	{ "PF_PHONET",       35 },
	{ "PF_IEEE802154",   36 },
	{ "PF_CAIF",         37 },
	{ "PF_ALG",          38 },
	{ "PF_NFC",          39 },
	{ "PF_VSOCK",        40 },
};

static const struct protocol *lookup_proto(const char *name, unsigned int proto)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(protocols); i++) {
		if ((name && strcmp(name, protocols[i].name) == 0) ||
		    (proto != -1u && protocols[i].proto == proto))
			return &protocols[i];
	}

	return NULL;
}

const char * get_proto_name(unsigned int proto)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(protocols); i++)
		if (protocols[i].proto == proto)
			return protocols[i].name;
	return NULL;
}

void find_specific_proto(const char *protoarg)
{
	const struct protocol *p;
	unsigned int i;

	p = lookup_proto(protoarg, specific_proto ? : -1u);
	if (p) {
		specific_proto = p->proto;
		output(2, "Using protocol %s (%u) for all sockets\n", p->name, p->proto);
		return;
	}

	outputerr("Protocol unknown. Pass a numeric value [0-%d] or one of ", TRINITY_PF_MAX);
	for (i = 0; i < ARRAY_SIZE(protocols); i++)
		outputerr("%s ", protocols[i].name);
	outputerr("\n");

	exit(EXIT_FAILURE);
}

unsigned int find_next_enabled_proto(unsigned int from)
{
	unsigned int i;

	from %= ARRAY_SIZE(no_protos);

	for (i = from; i < ARRAY_SIZE(no_protos); i++) {
		if (no_protos[i] == FALSE)
			return no_protos[i];
	}

	for (i = 0; i < from; i++) {
		if (no_protos[i] == FALSE)
			return no_protos[i];
	}

	return -1u;
}

void parse_exclude_protos(const char *arg)
{
	char *_arg = strdup(arg);
	const struct protocol *p;
	char *tok;

	if (!_arg) {
		outputerr("No free memory\n");
		exit(EXIT_FAILURE);
	}

	for (tok = strtok(_arg, ","); tok; tok = strtok(NULL, ",")) {
		p = lookup_proto(tok, (unsigned int)atoi(tok));
		if (p) {
			BUG_ON(p->proto >= ARRAY_SIZE(no_protos));
			no_protos[p->proto] = TRUE;
		} else
			goto err;
	}

	free(_arg);
	return;

err:
	free(_arg);
	outputerr("Protocol unknown in argument %s\n", arg);
	exit(EXIT_FAILURE);
}
