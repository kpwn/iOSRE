#include <linux/unistd.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "trinity.h"
#include "compat.h"
#include "random.h"
#include "syscall.h"
#include "log.h"
#include "net.h"
#include "tables.h"

/**
 * BPF filters are used in networking such as in pf_packet, but also
 * in seccomp for application sand-boxing. Additionally, with arch
 * specific BPF JIT compilers, this might be good to fuzz for errors.
 *    -- Daniel Borkmann, <borkmann@redhat.com>
 */

/* Both here likely defined in linux/filter.h already */
#ifndef SKF_AD_OFF
# define SKF_AD_OFF	(-0x1000)
#endif

#ifndef SKF_AD_MAX
# define SKF_AD_MAX	56
#endif

#define syscall_nr	(offsetof(struct seccomp_data, nr))
#define arch_nr		(offsetof(struct seccomp_data, arch))

#define SECCOMP_MODE_FILTER	2

#define BPF_CLASS(code) ((code) & 0x07)
#define	BPF_LD		0x00
#define	BPF_LDX		0x01
#define	BPF_ST		0x02
#define	BPF_STX		0x03
#define	BPF_ALU		0x04
#define	BPF_JMP		0x05
#define	BPF_RET		0x06
#define	BPF_MISC	0x07

static const uint16_t bpf_class_vars[] = {
	BPF_LD, BPF_LDX, BPF_ST, BPF_STX, BPF_ALU, BPF_JMP, BPF_RET, BPF_MISC,
};

#define BPF_SIZE(code)	((code) & 0x18)
#define	BPF_W		0x00
#define	BPF_H		0x08
#define	BPF_B		0x10

static const uint16_t bpf_size_vars[] = {
	BPF_W, BPF_H, BPF_B,
};

#define BPF_MODE(code)	((code) & 0xe0)
#define	BPF_IMM 	0x00
#define	BPF_ABS		0x20
#define	BPF_IND		0x40
#define	BPF_MEM		0x60
#define	BPF_LEN		0x80
#define	BPF_MSH		0xa0

static const uint16_t bpf_mode_vars[] = {
	BPF_IMM, BPF_ABS, BPF_IND, BPF_MEM, BPF_LEN, BPF_MSH,
};

#define BPF_OP(code)	((code) & 0xf0)
#define	BPF_ADD		0x00
#define	BPF_SUB		0x10
#define	BPF_MUL		0x20
#define	BPF_DIV		0x30
#define	BPF_OR		0x40
#define	BPF_AND		0x50
#define	BPF_LSH		0x60
#define	BPF_RSH		0x70
#define	BPF_NEG		0x80
#define BPF_MOD		0x90
#define	BPF_XOR		0xa0

static const uint16_t bpf_alu_op_vars[] = {
	BPF_ADD, BPF_SUB, BPF_MUL, BPF_DIV, BPF_OR, BPF_AND, BPF_LSH, BPF_RSH,
	BPF_NEG, BPF_MOD, BPF_XOR,
};

#define	BPF_JA		0x00
#define	BPF_JEQ		0x10
#define	BPF_JGT		0x20
#define	BPF_JGE		0x30
#define	BPF_JSET	0x40

static const uint16_t bpf_jmp_op_vars[] = {
	BPF_JA, BPF_JEQ, BPF_JGT, BPF_JGE, BPF_JSET,
};

#define BPF_SRC(code)	((code) & 0x08)
#define	BPF_K		0x00
#define	BPF_X		0x08

static const uint16_t bpf_src_vars[] = {
	BPF_K, BPF_X,
};

#define BPF_RVAL(code)	((code) & 0x18)
#define	BPF_A		0x10

static const uint16_t bpf_ret_vars[] = {
	BPF_A, BPF_K, BPF_X,
};

#define BPF_MISCOP(code) ((code) & 0xf8)
#define	BPF_TAX		0x00
#define	BPF_TXA		0x80

static const uint16_t bpf_misc_vars[] = {
	BPF_TAX, BPF_TXA,
};

#define SECCOMP_RET_KILL	0x00000000U
#define SECCOMP_RET_TRAP	0x00030000U
#define SECCOMP_RET_ALLOW	0x7fff0000U

static const uint32_t bpf_seccomp_ret_k_vars[] = {
	SECCOMP_RET_KILL, SECCOMP_RET_TRAP, SECCOMP_RET_ALLOW,
};

static const uint32_t bpf_seccomp_jmp_arch_vars[] = {
	AUDIT_ARCH_ALPHA, AUDIT_ARCH_ARM, AUDIT_ARCH_ARMEB, AUDIT_ARCH_CRIS,
	AUDIT_ARCH_FRV, AUDIT_ARCH_I386, AUDIT_ARCH_IA64,
	AUDIT_ARCH_M32R, AUDIT_ARCH_M68K, AUDIT_ARCH_MIPS, AUDIT_ARCH_MIPSEL,
	AUDIT_ARCH_MIPS64, AUDIT_ARCH_MIPSEL64, AUDIT_ARCH_PARISC,
	AUDIT_ARCH_PARISC64, AUDIT_ARCH_PPC, AUDIT_ARCH_PPC64, AUDIT_ARCH_S390,
	AUDIT_ARCH_S390X, AUDIT_ARCH_SH, AUDIT_ARCH_SHEL, AUDIT_ARCH_SH64,
	AUDIT_ARCH_SHEL64, AUDIT_ARCH_SPARC, AUDIT_ARCH_SPARC64,
	AUDIT_ARCH_X86_64,
};

#if defined(__i386__)
# define TRUE_REG_SYSCALL	REG_EAX
# define TRUE_ARCH		AUDIT_ARCH_I386
#elif defined(__x86_64__)
# define TRUE_REG_SYSCALL	REG_RAX
# define TRUE_ARCH		AUDIT_ARCH_X86_64
#else
# define TRUE_REG_SYSCALL	((uint32_t) rand()) /* TODO later */
# define TRUE_ARCH		((uint32_t) rand()) /* TODO later */
#endif

struct seccomp_data {
	int nr;
	uint32_t arch;
	uint64_t instruction_pointer;
	uint64_t args[6];
};

#define bpf_rand(type) \
	(bpf_##type##_vars[rand() % ARRAY_SIZE(bpf_##type##_vars)])

static uint16_t gen_bpf_code(bool last_instr)
{
	uint16_t ret = bpf_rand(class);

	if (last_instr) {
		/* The kernel filter precheck code already tests if
		 * there's a return instruction as the last one, so
		 * increase the chance to be accepted and that we
		 * actually run the generated fuzz filter code.
		 */
		if (rand_bool())
			ret = BPF_RET;
	}

	switch (ret) {
	case BPF_LD:
	case BPF_LDX:
	case BPF_ST:
	case BPF_STX:
		ret |= bpf_rand(size) | bpf_rand(mode) | bpf_rand(src);
		break;
	case BPF_ALU:
		ret |= bpf_rand(alu_op) | bpf_rand(src);
		break;
	case BPF_JMP:
		ret |= bpf_rand(jmp_op) | bpf_rand(src);
		break;
	case BPF_RET:
		ret |= bpf_rand(ret);
		break;
	case BPF_MISC:
		ret |= bpf_rand(misc);
		break;
	default:
		ret = (uint16_t) rand();
		break;
	}

	/* Also give it a chance to fuzz some crap into it */
	if (rand() % 10 == 0)
		ret |= (uint16_t) rand();

	return ret;
}

static int seccomp_state;

enum {
	STATE_GEN_VALIDATE_ARCH    = 0,
	STATE_GEN_EXAMINE_SYSCALL  = 1,
	STATE_GEN_ALLOW_SYSCALL    = 2,
	STATE_GEN_KILL_PROCESS     = 3,
	STATE_GEN_RANDOM_CRAP      = 4,
	__STATE_GEN_MAX,
};

#define STATE_GEN_MAX	(__STATE_GEN_MAX - 1)

static const float
seccomp_markov[__STATE_GEN_MAX][__STATE_GEN_MAX] = {
	{ .1f,	.5f,	.3f,	.05f,	.05f },
	{ .1f,	.3f,	.5f,	.05f,	.05f },
	{ .1f,	.3f,	.5f,	.05f,	.05f },
	{ .2f,	.2f,	.2f,	.2f,	.2f  },
	{ .2f,	.2f,	.2f,	.2f,	.2f  },
};

static const float seccomp_markov_init[__STATE_GEN_MAX] = {
	.5f, .3f, .1f, .05f, .05f
};

static int gen_seccomp_bpf_code(struct sock_filter *curr)
{
	int used = 0;
	struct sock_filter validate_arch[] = {
		BPF_STMT(BPF_LD  | BPF_W   | BPF_ABS, arch_nr),
		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0, 1, 0),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),
	};
	struct sock_filter examine_syscall[] = {
		BPF_STMT(BPF_LD | BPF_W | BPF_ABS, syscall_nr),
	};
	struct sock_filter allow_syscall[] = {
		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0, 0, 1),
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
	};
	struct sock_filter kill_process[] = {
		BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),
	};

	switch (seccomp_state) {
	case STATE_GEN_VALIDATE_ARCH:
		used = 3;
		memcpy(curr, validate_arch, sizeof(validate_arch));
		/* Randomize architecture */
		if (rand() % 3 == 0)
			curr[0].k = bpf_rand(seccomp_jmp_arch);
		else
			curr[0].k = TRUE_ARCH;
		break;
	case STATE_GEN_EXAMINE_SYSCALL:
		used = 1;
		memcpy(curr, examine_syscall, sizeof(examine_syscall));
		break;
	case STATE_GEN_ALLOW_SYSCALL:
		used = 2;
		memcpy(curr, allow_syscall, sizeof(allow_syscall));
		/* We assume here that max_nr_syscalls was computed before */
		curr[0].k = rand() % max_nr_syscalls;
		break;
	case STATE_GEN_KILL_PROCESS:
		used = 1;
		memcpy(curr, kill_process, sizeof(kill_process));
		if (rand() % 3 == 0)
			/* Variate between seccomp ret values */
			curr[0].k = bpf_rand(seccomp_ret_k);
		break;
	default:
	case STATE_GEN_RANDOM_CRAP:
		used = 1;
		curr->code = (uint16_t) rand();
		curr->jt = (uint8_t) rand();
		curr->jf = (uint8_t) rand();
		curr->k = rand32();
		break;
	}

	/* Also give it a chance to fuzz some crap into it */
	if (rand() % 10 == 0)
		curr[0].code |= (uint16_t) rand();
	if (rand() % 10 == 0)
		curr[1].code |= (uint16_t) rand();
	if (rand() % 10 == 0)
		curr[2].code |= (uint16_t) rand();

	return used;
}

static int seccomp_choose(const float probs[__STATE_GEN_MAX])
{
	int i;
	float sum = .0f;
	float thr = (float) rand() / (float) RAND_MAX;

	for (i = 0; i < __STATE_GEN_MAX; ++i) {
		sum += probs[i];
		if (sum > thr)
			return i;
	}

	BUG("wrong state\n");
	return -1;
}

void gen_seccomp_bpf(unsigned long *addr, unsigned long *addrlen)
{
	int avail, used;
	struct sock_filter *curr;
	struct sock_fprog *bpf = (void *) addr;

	if (addrlen != NULL) {
		bpf = malloc(sizeof(struct sock_fprog));
		if (bpf == NULL)
			return;
	}

	bpf->len = avail = rand() % BPF_MAXINSNS;

	bpf->filter = malloc(bpf->len * sizeof(struct sock_filter));
	if (bpf->filter == NULL) {
		if (addrlen != NULL)
			free(bpf);
		return;
	}

	memset(bpf->filter, 0, bpf->len * sizeof(struct sock_filter));

	seccomp_state = seccomp_choose(seccomp_markov_init);

	for (curr = bpf->filter; avail > 3; ) {
		used = gen_seccomp_bpf_code(curr);
		curr  += used;
		avail -= used;

		seccomp_state = seccomp_choose(seccomp_markov[seccomp_state]);
	}

	if (addrlen != NULL) {
		*addr = (unsigned long) bpf;
		*addrlen = sizeof(struct sock_fprog);
	}
}

void gen_bpf(unsigned long *addr, unsigned long *addrlen)
{
	int i;
	struct sock_fprog *bpf = (void *) addr;

	if (addrlen != NULL) {
		bpf = malloc(sizeof(struct sock_fprog));
		if (bpf == NULL)
			return;
	}

	bpf->len = rand() % BPF_MAXINSNS;

	bpf->filter = malloc(bpf->len * sizeof(struct sock_filter));
	if (bpf->filter == NULL) {
		if (addrlen != NULL)
			free(bpf);
		return;
	}

	for (i = 0; i < bpf->len; i++) {
		memset(&bpf->filter[i], 0, sizeof(bpf->filter[i]));

		bpf->filter[i].code = gen_bpf_code(i == bpf->len - 1);

		/* Fill out jump offsets if jmp instruction */
		if (BPF_CLASS(bpf->filter[i].code) == BPF_JMP) {
			bpf->filter[i].jt = (uint8_t) rand();
			bpf->filter[i].jf = (uint8_t) rand();
		}

		/* Also give it a chance if not BPF_JMP */
		if (rand() % 10 == 0)
			bpf->filter[i].jt |= (uint8_t) rand();
		if (rand() % 10 == 0)
			bpf->filter[i].jf |= (uint8_t) rand();

		/* Not always fill out k */
		bpf->filter[i].k = rand_bool() == 0 ? 0 : (uint32_t) rand();

		/* Also try to jump into BPF extensions by chance */
		if (BPF_CLASS(bpf->filter[i].code) == BPF_LD ||
		    BPF_CLASS(bpf->filter[i].code) == BPF_LDX) {
			if (bpf->filter[i].k > 65000 &&
			    bpf->filter[i].k < (uint32_t) SKF_AD_OFF) {
				if (rand_bool()) {
					bpf->filter[i].k = (uint32_t) (SKF_AD_OFF +
							   rand() % SKF_AD_MAX);
				}
			}
		}
	}

	if (addrlen != NULL) {
		*addr = (unsigned long) bpf;
		*addrlen = sizeof(struct sock_fprog);
	}
}
