#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <linux/ax25.h>
#include <stdlib.h>
#include "maps.h"	// page_rand
#include "net.h"
#include "random.h"

void ax25_gen_sockaddr(unsigned long *addr, unsigned long *addrlen)
{
	struct sockaddr_ax25 *ax25;

	ax25 = malloc(sizeof(struct sockaddr_ax25));
	if (ax25 == NULL)
		return;

	ax25->sax25_family = PF_AX25;
	memcpy(ax25->sax25_call.ax25_call, page_rand, 7);
	ax25->sax25_ndigis = rand();
	*addr = (unsigned long) ax25;
	*addrlen = sizeof(struct sockaddr_ax25);
}

#define NR_AX25_PROTOS 13
static int ax25_protocols[NR_AX25_PROTOS] = {
	0x01,   /* ROSE */
	0x06,   /* Compressed TCP/IP packet   *//* Van Jacobsen (RFC 1144)    */
	0x07,   /* Uncompressed TCP/IP packet *//* Van Jacobsen (RFC 1144)    */
	0x08,   /* Segmentation fragment      */
	0xc3,   /* TEXTNET datagram protocol  */
	0xc4,   /* Link Quality Protocol      */
	0xca,   /* Appletalk                  */
	0xcb,   /* Appletalk ARP              */
	0xcc,   /* ARPA Internet Protocol     */
	0xcd,   /* ARPA Address Resolution    */
	0xce,   /* FlexNet                    */
	0xcf,   /* NET/ROM                    */
	0xF0    /* No layer 3 protocol impl.  */
};

void ax25_rand_socket(struct socket_triplet *st)
{
	switch (rand() % 3) {
	case 0: st->type = SOCK_DGRAM;
		st->protocol = 0;
		break;
	case 1: st->type = SOCK_SEQPACKET;
		st->protocol = ax25_protocols[rand() % NR_AX25_PROTOS];
		break;
	case 2: st->type = SOCK_RAW;
		break;
	default:break;
	}
}
