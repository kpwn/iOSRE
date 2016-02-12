#ifndef _NET_H
#define _NET_H 1

#include <netinet/in.h>

extern unsigned int nr_sockets;

/* protocol decoding */
extern unsigned int specific_proto;


/* glibc headers might be older than the kernel, so chances are we know
 * about more protocols than glibc does. So we define our own PF_MAX */
#define TRINITY_PF_MAX 41

#define TYPE_MAX 10
#define PROTO_MAX 256

#define PF_NOHINT (-1)

struct socket_triplet {
	unsigned int family;
	unsigned int type;
	unsigned int protocol;
};

struct sockopt {
	unsigned int level;
	unsigned long optname;
	unsigned long optval;
	unsigned long optlen;
};

/* We create one of these per socket fd we open, and store them in shm->sockets */
struct socketinfo {
	struct socket_triplet triplet;
	int fd;
};

void open_sockets(void);
void close_sockets(void);

void generate_sockaddr(unsigned long *addr, unsigned long *addrlen, int pf);

void do_setsockopt(struct sockopt *so);

void rand_proto_type(struct socket_triplet *st);
int sanitise_socket_triplet(struct socket_triplet *st);
void gen_socket_args(struct socket_triplet *st);

/* Ethernet */
int get_random_ether_type(void);

/* ipv4 */
in_addr_t random_ipv4_address(void);
void ipv4_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void inet_rand_socket(struct socket_triplet *st);
void ip_setsockopt(struct sockopt *so);

/* ipv6 */
void ipv6_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void inet6_rand_socket(struct socket_triplet *st);
void inet6_setsockopt(struct sockopt *so);

/* pppox */
void pppox_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void pppol2tp_setsockopt(struct sockopt *so);

/* unix */
void unix_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void unix_rand_socket(struct socket_triplet *st);

/* bpf */
void gen_bpf(unsigned long *addr, unsigned long *addrlen);
void gen_seccomp_bpf(unsigned long *addr, unsigned long *addrlen);

/* caif */
void caif_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void caif_rand_socket(struct socket_triplet *st);
void caif_setsockopt(struct sockopt *so);

/* alg */
void alg_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void alg_setsockopt(struct sockopt *so);

/* nfc */
void nfc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void nfc_rand_socket(struct socket_triplet *st);
void nfc_setsockopt(struct sockopt *so);

/* ax25 */
void ax25_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void ax25_rand_socket(struct socket_triplet *st);
void ax25_setsockopt(struct sockopt *so);

/* ipx */
void ipx_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void ipx_rand_socket(struct socket_triplet *st);
void ipx_setsockopt(struct sockopt *so);

/* appletalk */
void atalk_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void atalk_rand_socket(struct socket_triplet *st);
void atalk_setsockopt(struct sockopt *so);

/* atm */
void atmpvc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void atmsvc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void atm_setsockopt(struct sockopt *so);

/* x25 */
void x25_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void x25_rand_socket(struct socket_triplet *st);
void x25_setsockopt(struct sockopt *so);

/* rose */
void rose_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void rose_setsockopt(struct sockopt *so);

/* decnet */
void decnet_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void decnet_rand_socket(struct socket_triplet *st);
void decnet_setsockopt(struct sockopt *so);

/* llc */
void llc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void llc_rand_socket(struct socket_triplet *st);
void llc_setsockopt(struct sockopt *so);

/* netlink */
void netlink_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void netlink_rand_socket(struct socket_triplet *st);
void netlink_setsockopt(struct sockopt *so);

/* packet */
void packet_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void packet_rand_socket(struct socket_triplet *st);
void packet_setsockopt(struct sockopt *so);

/* econet */
void econet_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);

/* irda */
void irda_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void irda_rand_socket(struct socket_triplet *st);
void irda_setsockopt(struct sockopt *so);

/* can */
void can_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void can_rand_socket(struct socket_triplet *st);

/* tipc */
void tipc_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void tipc_rand_socket(struct socket_triplet *st);
void tipc_setsockopt(struct sockopt *so);

/* phonet */
void phonet_gen_sockaddr(unsigned long *addr, unsigned long *addrlen);
void phonet_rand_socket(struct socket_triplet *st);

/* rds */
void rds_rand_socket(struct socket_triplet *st);
void rds_setsockopt(struct sockopt *so);

/* setsockopt functions */
void socket_setsockopt(struct sockopt *so);
void tcp_setsockopt(struct sockopt *so);
void udp_setsockopt(struct sockopt *so);
void udplite_setsockopt(struct sockopt *so);
void icmpv6_setsockopt(struct sockopt *so);
void sctp_setsockopt(struct sockopt *so);
void raw_setsockopt(struct sockopt *so);
void netrom_setsockopt(struct sockopt *so);
void aal_setsockopt(struct sockopt *so);
void netbeui_setsockopt(struct sockopt *so);
void dccp_setsockopt(struct sockopt *so);
void rxrpc_setsockopt(struct sockopt *so);
void bluetooth_setsockopt(struct sockopt *so);
void pnpipe_setsockopt(struct sockopt *so);
void iucv_setsockopt(struct sockopt *so);

#endif	/* _NET_H */
