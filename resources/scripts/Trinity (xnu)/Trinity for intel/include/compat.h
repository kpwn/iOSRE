typedef unsigned char __u8;
typedef unsigned int __u32;
typedef unsigned long long __u64;
#include <stdint.h>

#ifndef _TRINITY_COMPAT_H
#define _TRINITY_COMPAT_H 1

/* fcntl.h */
#ifndef AT_EMPTY_PATH
#define AT_EMPTY_PATH           0x1000
#endif

#ifndef O_PATH
#define O_PATH        010000000 /* Resolve pathname but do not open file.  */
#endif

#ifndef O_CLOEXEC
#define O_CLOEXEC       02000000
#endif

#ifndef O_TMPFILE
#define O_TMPFILE	020000000
#endif

#ifndef AT_NO_AUTOMOUNT
#define AT_NO_AUTOMOUNT 0x800
#endif

#ifndef F_LINUX_SPECIFIC_BASE
#define F_LINUX_SPECIFIC_BASE 1024
#endif

#ifndef F_SETPIPE_SZ
#define F_SETPIPE_SZ    (F_LINUX_SPECIFIC_BASE + 7)
#endif

#ifndef F_GETPIPE_SZ
#define F_GETPIPE_SZ    (F_LINUX_SPECIFIC_BASE + 8)
#endif

#ifndef F_DUPFD_CLOEXEC
#define F_DUPFD_CLOEXEC (F_LINUX_SPECIFIC_BASE + 6)
#endif

#ifndef F_SETOWN_EX
#define F_SETOWN_EX 15
#endif

#ifndef F_GETOWN_EX
#define F_GETOWN_EX 16
#endif

/* linux/hw_breakpoint.h */
enum {
        HW_BREAKPOINT_LEN_1 = 1,
        HW_BREAKPOINT_LEN_2 = 2,
        HW_BREAKPOINT_LEN_4 = 4,
        HW_BREAKPOINT_LEN_8 = 8,
};

enum {
        HW_BREAKPOINT_EMPTY     = 0,
        HW_BREAKPOINT_R         = 1,
        HW_BREAKPOINT_W         = 2,
        HW_BREAKPOINT_RW        = HW_BREAKPOINT_R | HW_BREAKPOINT_W,
        HW_BREAKPOINT_X         = 4,
        HW_BREAKPOINT_INVALID   = HW_BREAKPOINT_RW | HW_BREAKPOINT_X,
};

/* asm-generic/mman-common.h */

#ifndef MAP_UNINITIALIZED
#define MAP_UNINITIALIZED 0x4000000
#endif
#ifndef PROT_SEM
#define PROT_SEM 0x8
#endif
#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0x40000
#endif
#ifndef MAP_STACK
#define MAP_STACK 0x20000
#endif

#ifndef MADV_MERGEABLE
#define MADV_MERGEABLE 12
#endif
#ifndef MADV_UNMERGEABLE
#define MADV_UNMERGEABLE 13
#endif
#ifndef MADV_HUGEPAGE
#define MADV_HUGEPAGE 14
#endif
#ifndef MADV_NOHUGEPAGE
#define MADV_NOHUGEPAGE 15
#endif


/* bits/socket.h */
#ifndef SOCK_CLOEXEC
#define SOCK_CLOEXEC	02000000
#endif

#ifndef SOCK_NONBLOCK
#define SOCK_NONBLOCK	04000
#endif

#ifndef PF_RDS
#define PF_RDS		21
#endif
#ifndef AF_RDS
#define AF_RDS		PF_RDS
#endif

#ifndef PF_LLC
#define PF_LLC		26
#endif
#ifndef AF_LLC
#define AF_LLC		PF_LLC
#endif

#ifndef AF_IB
#define AF_IB		27
#endif
#ifndef PF_IB
#define PF_IB          AF_IB
#endif

#ifndef PF_CAN
#define PF_CAN		29
#endif
#ifndef AF_CAN
#define AF_CAN		PF_CAN
#endif

#ifndef PF_TIPC
#define PF_TIPC		30
#endif
#ifndef AF_TIPC
#define AF_TIPC		PF_TIPC
#endif

#ifndef PF_PHONET
#define PF_PHONET	35
#endif
#ifndef AF_PHONET
#define AF_PHONET	PF_PHONET
#endif

#ifndef PF_CAIF
#define PF_CAIF		37
#endif
#ifndef AF_CAIF
#define AF_CAIF		PF_CAIF
#endif

#ifndef PF_ALG
#define PF_ALG		38
#endif
#ifndef AF_ALG
#define AF_ALG		PF_ALG
#endif

#ifndef PF_NFC
#define PF_NFC		39
#endif
#ifndef AF_NFC
#define AF_NFC		PF_NFC
#endif

#ifndef PF_VSOCK
#define PF_VSOCK        40
#endif
#ifndef AF_VSOCK
#define AF_VSOCK PF_VSOCK
#endif

#ifndef NFC_SOCKPROTO_RAW
#define NFC_SOCKPROTO_RAW	0
#endif
#ifndef NFC_SOCKPROTO_LLCP
#define NFC_SOCKPROTO_LLCP	1
#endif

#ifndef MSG_WAITFORONE
#define MSG_WAITFORONE	0x10000
#endif

#ifndef MSG_CMSG_CLOEXEC
#define MSG_CMSG_CLOEXEC	0x40000000
#endif

/* linux/socket.h */
#ifndef MSG_PROBE
#define MSG_PROBE 0x10
#endif
#ifndef MSG_FASTOPEN
#define MSG_FASTOPEN 0x20000000
#endif
#ifndef MSG_CMSG_COMPAT
#define MSG_CMSG_COMPAT 0x80000000
#endif

/* linux/net.h */
#ifndef SYS_RECVMMSG
#define SYS_RECVMMSG 19
#endif
#ifndef SYS_SENDMMSG
#define SYS_SENDMMSG 20
#endif

/* linux/netlink.h */
#ifndef NETLINK_CRYPTO
#define NETLINK_CRYPTO 21
#endif
#ifndef NETLINK_RX_RING
#define NETLINK_RX_RING 6
#define NETLINK_TX_RING 7
#endif

/* linux/prctl.h */
#ifndef PR_MCE_KILL_GET
#define PR_MCE_KILL_GET 34
#endif

/* linux/rds.h */
#ifndef RDS_CANCEL_SENT_TO
#define RDS_CANCEL_SENT_TO              1
#define RDS_GET_MR                      2
#define RDS_FREE_MR                     3
/* deprecated: RDS_BARRIER 4 */
#define RDS_RECVERR                     5
#define RDS_CONG_MONITOR                6
#define RDS_GET_MR_FOR_DEST             7
#endif

/* asm/ptrace-abi.h */
#ifndef PTRACE_SYSEMU
#define PTRACE_SYSEMU		  31
#endif
#ifndef PTRACE_SYSEMU_SINGLESTEP
#define PTRACE_SYSEMU_SINGLESTEP  32
#endif
#ifndef PTRACE_GETSIGMASK
#define PTRACE_GETSIGMASK	0x420a
#define PTRACE_SETSIGMASK	0x420b
#endif

/* sys/timerfd.h */
#ifndef TFD_CLOEXEC
#define TFD_CLOEXEC 02000000
#endif
#ifndef TFD_NONBLOCK
#define TFD_NONBLOCK 04000
#endif

/* linux/keyctl.h */
#ifndef KEYCTL_GET_KEYRING_ID
#define KEYCTL_GET_KEYRING_ID		0	/* ask for a keyring's ID */
#define KEYCTL_JOIN_SESSION_KEYRING	1	/* join or start named session keyring */
#define KEYCTL_UPDATE			2	/* update a key */
#define KEYCTL_REVOKE			3	/* revoke a key */
#define KEYCTL_CHOWN			4	/* set ownership of a key */
#define KEYCTL_SETPERM			5	/* set perms on a key */
#define KEYCTL_DESCRIBE			6	/* describe a key */
#define KEYCTL_CLEAR			7	/* clear contents of a keyring */
#define KEYCTL_LINK			8	/* link a key into a keyring */
#define KEYCTL_UNLINK			9	/* unlink a key from a keyring */
#define KEYCTL_SEARCH			10	/* search for a key in a keyring */
#define KEYCTL_READ			11	/* read a key or keyring's contents */
#define KEYCTL_INSTANTIATE		12	/* instantiate a partially constructed key */
#define KEYCTL_NEGATE			13	/* negate a partially constructed key */
#define KEYCTL_SET_REQKEY_KEYRING	14	/* set default request-key keyring */
#define KEYCTL_SET_TIMEOUT		15	/* set key timeout */
#define KEYCTL_ASSUME_AUTHORITY		16	/* assume request_key() authorisation */
#define KEYCTL_GET_SECURITY		17	/* get key security label */
#define KEYCTL_SESSION_TO_PARENT	18	/* apply session keyring to parent process */
#endif

#ifndef KEYCTL_REJECT
#define KEYCTL_REJECT			19	/* reject a partially constructed key */
#endif

#ifndef KEYCTL_INSTANTIATE_IOV
#define KEYCTL_INSTANTIATE_IOV		20	/* instantiate a partially constructed key */
#endif

#ifndef KCMP_TYPES
enum kcmp_type {
	KCMP_FILE,
	KCMP_VM,
	KCMP_FILES,
	KCMP_FS,
	KCMP_SIGHAND,
	KCMP_IO,
	KCMP_SYSVSEM,

	KCMP_TYPES,
};
#endif

/* asm/socket.h */
#ifndef SO_BSDCOMPAT
#define SO_BSDCOMPAT		14
#endif

#ifndef SO_REUSEPORT
#define SO_REUSEPORT		15
#endif

#ifndef SO_RXQ_OVFL
#define SO_RXQ_OVFL		40
#endif

#ifndef SO_WIFI_STATUS
#define SO_WIFI_STATUS		41
#endif

#ifndef SO_PEEK_OFF
#define SO_PEEK_OFF		42
#endif

#ifndef SO_NOFCS
#define SO_NOFCS		43
#endif

#ifndef SO_LOCK_FILTER
#define SO_LOCK_FILTER		44
#endif

#ifndef SO_SELECT_ERR_QUEUE
#define SO_SELECT_ERR_QUEUE	45
#endif

#ifndef SO_BUSY_POLL
#define SO_BUSY_POLL		46
#endif

#ifndef SO_MAX_PACING_RATE
#define SO_MAX_PACING_RATE	47
#endif

/* linux/tcp.h */
#ifndef TCP_COOKIE_TRANSACTIONS
#define TCP_COOKIE_TRANSACTIONS	15
#endif

#ifndef TCP_THIN_LINEAR_TIMEOUTS
#define TCP_THIN_LINEAR_TIMEOUTS 16
#endif

#ifndef TCP_THIN_DUPACK
#define TCP_THIN_DUPACK		17
#endif

#ifndef TCP_USER_TIMEOUT
#define TCP_USER_TIMEOUT	18
#endif

#ifndef TCP_REPAIR
#define TCP_REPAIR		19
#endif

#ifndef TCP_REPAIR_QUEUE
#define TCP_REPAIR_QUEUE	20
#endif

#ifndef TCP_QUEUE_SEQ
#define TCP_QUEUE_SEQ		21
#endif

#ifndef TCP_REPAIR_OPTIONS
#define TCP_REPAIR_OPTIONS	22
#endif

#ifndef TCP_FASTOPEN
#define TCP_FASTOPEN		23
#endif

/* linux/if_packet.h */
#ifndef PACKET_VNET_HDR
#define PACKET_VNET_HDR		15
#endif

#ifndef PACKET_TX_TIMESTAMP
#define PACKET_TX_TIMESTAMP	16
#endif

#ifndef PACKET_TIMESTAMP
#define PACKET_TIMESTAMP	17
#endif

#ifndef PACKET_FANOUT
#define PACKET_FANOUT		18
#endif

/* linux/dccp.h */
#ifndef DCCP_SOCKOPT_QPOLICY_ID
#define DCCP_SOCKOPT_QPOLICY_ID	16
#endif

#ifndef DCCP_SOCKOPT_QPOLICY_TXQLEN
#define DCCP_SOCKOPT_QPOLICY_TXQLEN 17
#endif

/* net/udplite.h */
#ifndef UDPLITE_SEND_CSCOV
#define UDPLITE_SEND_CSCOV   10 /* sender partial coverage (as sent)      */
#endif
#ifndef UDPLITE_RECV_CSCOV
#define UDPLITE_RECV_CSCOV   11 /* receiver partial coverage (threshold ) */
#endif

/* linux/in.h */
#ifndef IP_MTU
#define IP_MTU			14
#endif
#ifndef IP_FREEBIND
#define IP_FREEBIND		15
#endif
#ifndef IP_IPSEC_POLICY
#define IP_IPSEC_POLICY		16
#endif
#ifndef IP_XFRM_POLICY
#define IP_XFRM_POLICY		17
#endif
#ifndef IP_PASSSEC
#define IP_PASSSEC		18
#endif
#ifndef IP_TRANSPARENT
#define IP_TRANSPARENT		19
#endif
#ifndef IP_MINTTL
#define IP_MINTTL		21
#endif
#ifndef IP_ORIGDSTADDR
#define IP_ORIGDSTADDR		20
#endif
#ifndef IP_RECVORIGDSTADDR
#define IP_RECVORIGDSTADDR	IP_ORIGDSTADDR
#endif
#ifndef IP_NODEFRAG
#define IP_NODEFRAG		22
#endif
#ifndef IP_MULTICAST_ALL
#define IP_MULTICAST_ALL	49
#endif
#ifndef IP_UNICAST_IF
#define IP_UNICAST_IF		50
#endif

/* linux/in6.h */
#ifndef IPV6_FLOWINFO
#define IPV6_FLOWINFO 11
#endif

/* asm/resource.h */
#ifndef RLIMIT_RTTIME
#define RLIMIT_RTTIME		15
#endif

/* sctp/user.h */
#ifndef SCTP_RTOINFO
#define SCTP_RTOINFO    0
#define SCTP_ASSOCINFO  1
#define SCTP_INITMSG    2
#define SCTP_NODELAY    3               /* Get/set nodelay option. */
#define SCTP_AUTOCLOSE  4
#define SCTP_SET_PEER_PRIMARY_ADDR 5
#define SCTP_PRIMARY_ADDR       6
#define SCTP_ADAPTATION_LAYER   7
#define SCTP_DISABLE_FRAGMENTS  8
#define SCTP_PEER_ADDR_PARAMS   9
#define SCTP_DEFAULT_SEND_PARAM 10
#define SCTP_EVENTS     11
#define SCTP_I_WANT_MAPPED_V4_ADDR 12   /* Turn on/off mapped v4 addresses  */
#define SCTP_MAXSEG     13              /* Get/set maximum fragment. */
#define SCTP_STATUS     14
#define SCTP_GET_PEER_ADDR_INFO 15
#define SCTP_DELAYED_ACK_TIME   16
#define SCTP_CONTEXT    17
#define SCTP_FRAGMENT_INTERLEAVE        18
#define SCTP_PARTIAL_DELIVERY_POINT     19 /* Set/Get partial delivery point */
#define SCTP_MAX_BURST  20              /* Set/Get max burst */
#define SCTP_AUTH_CHUNK 21      /* Set only: add a chunk type to authenticate */
#define SCTP_HMAC_IDENT 22
#define SCTP_AUTH_KEY   23
#define SCTP_AUTH_ACTIVE_KEY    24
#define SCTP_AUTH_DELETE_KEY    25
#define SCTP_PEER_AUTH_CHUNKS   26      /* Read only */
#define SCTP_LOCAL_AUTH_CHUNKS  27      /* Read only */
#define SCTP_GET_ASSOC_NUMBER   28      /* Read only */
#define SCTP_GET_ASSOC_ID_LIST  29      /* Read only */
#define SCTP_AUTO_ASCONF       30
#define SCTP_PEER_ADDR_THLDS    31
#endif

#ifndef SCTP_SOCKOPT_BINDX_ADD
#define SCTP_SOCKOPT_BINDX_ADD  100     /* BINDX requests for adding addrs */
#define SCTP_SOCKOPT_BINDX_REM  101     /* BINDX requests for removing addrs. */
#define SCTP_SOCKOPT_PEELOFF    102     /* peel off association. */
#define SCTP_SOCKOPT_CONNECTX_OLD       107     /* CONNECTX old requests. */
#define SCTP_GET_PEER_ADDRS     108             /* Get all peer address. */
#define SCTP_GET_LOCAL_ADDRS    109             /* Get all local address. */
#define SCTP_SOCKOPT_CONNECTX   110             /* CONNECTX requests. */
#define SCTP_SOCKOPT_CONNECTX3  111     /* CONNECTX requests (updated) */
#define SCTP_GET_ASSOC_STATS    112	/* Read only */
#endif

/* linux/rxrpc.h */
#ifndef RXRPC_USER_CALL_ID
#define RXRPC_USER_CALL_ID      1       /* user call ID specifier */
#define RXRPC_ABORT             2       /* abort request / notification [terminal] */
#define RXRPC_ACK               3       /* [Server] RPC op final ACK received [terminal] */
#define RXRPC_NET_ERROR         5       /* network error received [terminal] */
#define RXRPC_BUSY              6       /* server busy received [terminal] */
#define RXRPC_LOCAL_ERROR       7       /* local error generated [terminal] */
#define RXRPC_NEW_CALL          8       /* [Server] new incoming call notification */
#define RXRPC_ACCEPT            9       /* [Server] accept request */
#endif

/* net/bluetooth/bluetooth.h */
#ifndef BT_SECURITY
#define BT_SECURITY     4
#define BT_DEFER_SETUP  7
#define BT_FLUSHABLE    8
#define BT_POWER        9
#define BT_CHANNEL_POLICY       10

#define SOL_HCI         0
#define SOL_L2CAP       6
#define SOL_SCO         17
#define SOL_RFCOMM      18
#endif

/* net/bluetooth/hci.h */
#ifndef HCI_DATA_DIR
#define HCI_DATA_DIR    1
#define HCI_FILTER      2
#define HCI_TIME_STAMP  3
#endif

/* net/bluetooth/l2cap.h */
#ifndef L2CAP_OPTIONS
#define L2CAP_OPTIONS   0x01
#define L2CAP_LM        0x03
#endif

/* net/bluetooth/rfcomm.h */
#ifndef RFCOMM_LM
#define RFCOMM_LM       0x03
#endif

/* net/iucv/af_iucv.h */
#ifndef SO_IPRMDATA_MSG
#define SO_IPRMDATA_MSG 0x0080          /* send/recv IPRM_DATA msgs */
#define SO_MSGLIMIT     0x1000          /* get/set IUCV MSGLIMIT */
#define SO_MSGSIZE      0x0800          /* get maximum msgsize */
#endif

/* linux/nfc.h */
#ifndef sockaddr_nfc
//#include <bits/sockaddr.h>
//#include <linux/types.h>

struct sockaddr_nfc {
	sa_family_t sa_family;
	__u32 dev_idx;
	__u32 target_idx;
	__u32 nfc_protocol;
};
#endif

/* linux/inotify.h */
#ifndef IN_EXCL_UNLINK
#define IN_EXCL_UNLINK	0x04000000	/* exclude events on unlinked objects */
#endif

#ifndef MSG_COPY
#define MSG_COPY        040000
#endif

#ifndef MS_SNAP_STABLE
#define MS_SNAP_STABLE	(1<<27)
#endif

#ifndef MS_NOSEC
#define MS_NOSEC        (1<<28)
#endif

#ifndef MS_BORN
#define MS_BORN		(1<<29)
#endif

/* linux/kvm.h */
#ifndef KVM_GET_REG_LIST
struct kvm_reg_list {
        __u64 n; /* number of regs */
        __u64 reg[0];
};
#define KVM_GET_REG_LIST          _IOWR(KVMIO, 0xb0, struct kvm_reg_list)
#endif

#ifndef KVM_S390_UCAS_MAP
struct kvm_s390_ucas_mapping {
        __u64 user_addr;
        __u64 vcpu_addr;
        __u64 length;
};
#define KVM_S390_UCAS_MAP         _IOW(KVMIO, 0x50, struct kvm_s390_ucas_mapping)
#endif

#ifndef KVM_S390_UCAS_UNMAP
#define KVM_S390_UCAS_UNMAP       _IOW(KVMIO, 0x51, struct kvm_s390_ucas_mapping)
#endif

#ifndef KVM_S390_VCPU_FAULT
#define KVM_S390_VCPU_FAULT       _IOW(KVMIO, 0x52, unsigned long)
#endif

#ifndef KVM_XEN_HVM_CONFIG
struct kvm_xen_hvm_config {
	__u32 flags;
	__u32 msr;
	__u64 blob_addr_32;
	__u64 blob_addr_64;
	__u8 blob_size_32;
	__u8 blob_size_64;
	__u8 pad2[30];
};
#define KVM_XEN_HVM_CONFIG        _IOW(KVMIO,  0x7a, struct kvm_xen_hvm_config)
#endif

#ifndef KVM_PPC_GET_PVINFO
struct kvm_ppc_pvinfo {
	/* out */
	__u32 flags;
	__u32 hcall[4];
	__u8  pad[108];
};
#define KVM_PPC_GET_PVINFO        _IOW(KVMIO,  0xa1, struct kvm_ppc_pvinfo)
#endif

#ifndef KVM_SET_TSC_KHZ
#define KVM_SET_TSC_KHZ           _IO(KVMIO,  0xa2)
#endif

#ifndef KVM_GET_TSC_KHZ
#define KVM_GET_TSC_KHZ           _IO(KVMIO,  0xa3)
#endif

#ifndef KVM_ASSIGN_SET_INTX_MASK
#define KVM_ASSIGN_SET_INTX_MASK  _IOW(KVMIO,  0xa4, struct kvm_assigned_pci_dev)
#endif

#ifndef KVM_GET_DEBUGREGS
struct kvm_debugregs {
	__u64 db[4];
	__u64 dr6;
	__u64 dr7;
	__u64 flags;
	__u64 reserved[9];
};
#define KVM_GET_DEBUGREGS         _IOR(KVMIO,  0xa1, struct kvm_debugregs)
#define KVM_SET_DEBUGREGS         _IOW(KVMIO,  0xa2, struct kvm_debugregs)
#endif

#ifndef KVM_ENABLE_CAP
struct kvm_enable_cap {
	/* in */
	__u32 cap;
	__u32 flags;
	__u64 args[4];
	__u8  pad[64];
};
#define KVM_ENABLE_CAP            _IOW(KVMIO,  0xa3, struct kvm_enable_cap)
#endif

#ifndef KVM_GET_XSAVE
struct kvm_xsave {
	__u32 region[1024];
};
#define KVM_GET_XSAVE             _IOR(KVMIO,  0xa4, struct kvm_xsave)
#define KVM_SET_XSAVE             _IOW(KVMIO,  0xa5, struct kvm_xsave)
#endif

#ifndef KVM_GET_XCRS
#define KVM_MAX_XCRS    16
struct kvm_xcr {
	__u32 xcr;
	__u32 reserved;
	__u64 value;
};

struct kvm_xcrs {
	__u32 nr_xcrs;
	__u32 flags;
	struct kvm_xcr xcrs[KVM_MAX_XCRS];
	__u64 padding[16];
};
#define KVM_GET_XCRS              _IOR(KVMIO,  0xa6, struct kvm_xcrs)
#define KVM_SET_XCRS              _IOW(KVMIO,  0xa7, struct kvm_xcrs)
#endif

#ifndef KVM_SIGNAL_MSI
struct kvm_msi {
        __u32 address_lo;
        __u32 address_hi;
        __u32 data;
        __u32 flags;
        __u8  pad[16];
};
#define KVM_SIGNAL_MSI            _IOW(KVMIO,  0xa5, struct kvm_msi)
#endif

#ifndef KVM_DIRTY_TLB
struct kvm_dirty_tlb {
        __u64 bitmap;
        __u32 num_dirty;
};
#define KVM_DIRTY_TLB             _IOW(KVMIO,  0xaa, struct kvm_dirty_tlb)
#endif

#ifndef KVM_GET_ONE_REG
struct kvm_one_reg {
        __u64 id;
        __u64 addr;
};
#define KVM_GET_ONE_REG           _IOW(KVMIO,  0xab, struct kvm_one_reg)
#endif

#ifndef KVM_SET_ONE_REG
#define KVM_SET_ONE_REG           _IOW(KVMIO,  0xac, struct kvm_one_reg)
#endif

#ifndef KVM_KVMCLOCK_CTRL
#define KVM_KVMCLOCK_CTRL         _IO(KVMIO,   0xad)
#endif

#ifndef KVM_PPC_GET_SMMU_INFO
#define KVM_PPC_PAGE_SIZES_MAX_SZ	8

struct kvm_ppc_one_page_size {
	__u32 page_shift;	/* Page shift (or 0) */
	__u32 pte_enc;		/* Encoding in the HPTE (>>12) */
};

struct kvm_ppc_one_seg_page_size {
	__u32 page_shift;	/* Base page shift of segment (or 0) */
	__u32 slb_enc;		/* SLB encoding for BookS */
	struct kvm_ppc_one_page_size enc[KVM_PPC_PAGE_SIZES_MAX_SZ];
};

struct kvm_ppc_smmu_info {
	__u64 flags;
	__u32 slb_size;
	__u32 pad;
	struct kvm_ppc_one_seg_page_size sps[KVM_PPC_PAGE_SIZES_MAX_SZ];
};
#define KVM_PPC_GET_SMMU_INFO	  _IOR(KVMIO,  0xa6, struct kvm_ppc_smmu_info)
#endif

#ifndef KVM_PPC_ALLOCATE_HTAB
#define KVM_PPC_ALLOCATE_HTAB	  _IOWR(KVMIO, 0xa7, __u32)
#endif

#ifndef KVM_PPC_GET_HTAB_FD
struct kvm_get_htab_fd {
	__u64	flags;
	__u64	start_index;
	__u64	reserved[2];
};
#define KVM_PPC_GET_HTAB_FD	  _IOW(KVMIO,  0xaa, struct kvm_get_htab_fd)
#endif

#ifndef EM_ARM
#define EM_ARM                    40
#endif

/* linux/mroute.h */
#ifndef MRT_TABLE
#define MRT_TABLE		(MRT_BASE+9)
#endif
#ifndef MRT_ADD_MFC_PROXY
#define MRT_ADD_MFC_PROXY	(MRT_BASE+10)
#endif
#ifndef MRT_DEL_MFC_PROXY
#define MRT_DEL_MFC_PROXY	(MRT_BASE+11)
#endif

/* sys/mount.h */
#ifndef MNT_DETACH
#define MNT_DETACH		2
#endif

#ifndef MNT_EXPIRE
#define MNT_EXPIRE		4
#endif

#ifndef UMOUNT_NOFOLLOW
#define UMOUNT_NOFOLLOW		8
#endif

/* if_ether.h */
#ifndef ETH_P_CANFD
#define ETH_P_CANFD	0x000D
#endif
#ifndef ETH_P_CAIF
#define ETH_P_CAIF	0x00F7
#endif
#ifndef ETH_P_802_3_MIN
#define ETH_P_802_3_MIN	0x0600
#endif
#ifndef ETH_P_BATMAN
#define ETH_P_BATMAN	0x4305
#endif
#ifndef ETH_P_LINK_CTL
#define ETH_P_LINK_CTL	0x886c
#endif
#ifndef ETH_P_8021AD
#define ETH_P_8021AD	0x88A8
#endif
#ifndef ETH_P_802_EX1
#define ETH_P_802_EX1	0x88B5
#endif
#ifndef ETH_P_8021AH
#define ETH_P_8021AH	0x88E7
#endif
#ifndef ETH_P_MVRP
#define ETH_P_MVRP	0x88F5
#endif
#ifndef ETH_P_PRP
#define ETH_P_PRP	0x88FB
#endif
#ifndef ETH_P_QINQ1
#define ETH_P_QINQ1	0x9100
#endif
#ifndef ETH_P_QINQ2
#define ETH_P_QINQ2	0x9200
#endif
#ifndef ETH_P_QINQ3
#define ETH_P_QINQ3	0x9300
#endif

#endif	/* _TRINITY_COMPAT_H */
