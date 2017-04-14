#include "net.h"

void rds_rand_socket(struct socket_triplet *st)
{
	st->protocol = 0;
	st->type = SOCK_SEQPACKET;
}
