/*
 * SYSCALL_DEFINE5(perf_event_open,
	 struct perf_event_attr __user *, attr_uptr,
	 pid_t, pid, int, cpu, int, group_fd, unsigned long, flags)
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "perf_event.h"
#include "random.h"
#include "sanitise.h"
#include "maps.h"
#include "shm.h"
#include "log.h"
#include "compat.h"

#define SYSFS "/sys/bus/event_source/devices/"

struct generic_event_type {
	char *name;
	char *value;
	long long config;
	long long config1;
	long long config2;
};

struct format_type {
	char *name;
	char *value;
	int field;
	unsigned long long mask;
};

struct pmu_type {
	char *name;
	int type;
	int num_formats;
	int num_generic_events;
	struct format_type *formats;
	struct generic_event_type *generic_events;
};

static int num_pmus=0;

static struct pmu_type *pmus=NULL;


#define FIELD_UNKNOWN	0
#define FIELD_CONFIG	1
#define FIELD_CONFIG1	2
#define FIELD_CONFIG2	3
#define MAX_FIELDS	4


static int parse_format(char *string, int *field_type, unsigned long long *mask) {

	int i,firstnum,secondnum,shift,bits;
	char format_string[BUFSIZ];

	*mask=0;

	/* get format */
	/* according to Documentation/ABI/testing/sysfs-bus-event_source-devices-format */
	/* the format is something like config1:1,6-10,44 */

	i=0;
	while(1) {
		format_string[i]=string[i];
		if (string[i]==':') {
			format_string[i]=0;
			break;
		}
		if (string[i]==0) break;
		i++;
	}

	if (!strcmp(format_string,"config")) {
		*field_type=FIELD_CONFIG;
	} else if (!strcmp(format_string,"config1")) {
		*field_type=FIELD_CONFIG1;
	} else if (!strcmp(format_string,"config2")) {
		*field_type=FIELD_CONFIG2;
	}
	else {
		*field_type=FIELD_UNKNOWN;
	}

	while(1) {

		/* Read first number */
		i++;
		firstnum=0;
		while(1) {
			if (string[i]==0) break;
			if (string[i]=='-') break;
			if (string[i]==',') break;
			if ((string[i]<'0') || (string[i]>'9')) {
				outputerr("Unknown format char %c\n", string[i]);
				return -1;
			}
			firstnum*=10;
			firstnum+=(string[i])-'0';
			i++;
		}
		shift=firstnum;

		/* check if no second num */
		if ((string[i]==0) || (string[i]==',')) {
			bits=1;
		}
		else {
			/* Read second number */
			i++;
			secondnum=0;
			while(1) {
				if (string[i]==0) break;
				if (string[i]=='-') break;
				if (string[i]==',') break;
				if ((string[i]<'0') || (string[i]>'9')) {
					outputerr("Unknown format char %c\n", string[i]);
					return -1;
				}
				secondnum*=10;
				secondnum+=(string[i])-'0';
				i++;
			}
			bits=(secondnum-firstnum)+1;
		}

		if (bits==64) {
			*mask|=0xffffffffffffffffULL;
		} else {
			*mask|=((1ULL<<bits)-1)<<shift;
		}

		if (string[i]==0) break;

	}
	return 0;
}

static unsigned long long separate_bits(unsigned long long value,
					unsigned long long mask) {

	int value_bit=0,i;
	unsigned long long result=0;

	for(i=0;i<64;i++) {
		if ((1ULL<<i)&mask) {
			result|=((value>>value_bit)&1)<<i;
			value_bit++;
		}
	}

	return result;
}

static int update_configs(int pmu, char *field,
			long long value,
			long long *c,
			long long *c1,
			long long *c2) {

	int i;

	for(i=0;i<pmus[pmu].num_formats;i++) {
		if (!strcmp(field,pmus[pmu].formats[i].name)) {
			if (pmus[pmu].formats[i].field==FIELD_CONFIG) {
				*c|=separate_bits(value,
						pmus[pmu].formats[i].mask);
				return 0;
			}

			if (pmus[pmu].formats[i].field==FIELD_CONFIG1) {
				*c1|=separate_bits(value,
						pmus[pmu].formats[i].mask);
				return 0;
			}

			if (pmus[pmu].formats[i].field==FIELD_CONFIG2) {
				*c2|=separate_bits(value,
						pmus[pmu].formats[i].mask);
				return 0;
			}

		}
	}

	return 0;
}

static int parse_generic(int pmu, char *value,
			long long *config, long long *config1, long long *config2) {

	long long c=0,c1=0,c2=0,temp;
	char field[BUFSIZ];
	int i,ptr=0;
	int base=10;

	while(1) {
		i=0;
		while(1) {
			field[i]=value[ptr];
			if (value[ptr]==0) break;
			if ((value[ptr]=='=') || (value[ptr]==',')) {
				field[i]=0;
				break;
			}
			i++;
			ptr++;
		}

		/* if at end, was parameter w/o value */
		/* So it is a flag with a value of 1  */
		if ((value[ptr]==',') || (value[ptr]==0)) {
			temp=0x1;
		}
		else {
			/* get number */

			base=10;

			ptr++;

			if (value[ptr]=='0') {
				if (value[ptr+1]=='x') {
					ptr++;
					ptr++;
					base=16;
				}
			}
			temp=0x0;
			while(1) {

				if (value[ptr]==0) break;
				if (value[ptr]==',') break;
				if (! ( ((value[ptr]>='0') && (value[ptr]<='9'))
                   			|| ((value[ptr]>='a') && (value[ptr]<='f'))) ) {
					outputerr("Unexpected char %c\n", value[ptr]);
				}
				temp*=base;
				if ((value[ptr]>='0') && (value[ptr]<='9')) {
					temp+=value[ptr]-'0';
				}
				else {
					temp+=(value[ptr]-'a')+10;
				}
				i++;
				ptr++;
			}
		}
		update_configs(pmu,field,temp,&c,&c1,&c2);
		if (value[ptr]==0) break;
		ptr++;
	}
	*config=c;
	*config1=c1;
	*config2=c2;
	return 0;
}


static int init_pmus(void) {

	DIR *dir,*event_dir,*format_dir;
	struct dirent *entry,*event_entry,*format_entry;
	char dir_name[BUFSIZ] = "";
	char event_name[BUFSIZ] = "";
	char event_value[BUFSIZ] = "";
	char temp_name[BUFSIZ] = "";
	char format_name[BUFSIZ] = "";
	char format_value[BUFSIZ] = "";
	int type,pmu_num=0,format_num=0,generic_num=0;
	FILE *fff;
	int result = -1;


	/* Count number of PMUs */
	/* This may break if PMUs are ever added/removed on the fly? */

	dir=opendir(SYSFS);
	if (dir==NULL) {
		return -1;
	}

	while(1) {
		entry=readdir(dir);
		if (entry==NULL) break;
		if (!strcmp(".",entry->d_name)) continue;
		if (!strcmp("..",entry->d_name)) continue;
		num_pmus++;
	}

	if (num_pmus<1)
		goto out;

	pmus=calloc(num_pmus,sizeof(struct pmu_type));
	if (pmus==NULL)
		goto out;

	/****************/
	/* Add each PMU */
	/****************/

	rewinddir(dir);

	while(1) {
		entry=readdir(dir);
		if (entry==NULL) break;
		if (!strcmp(".",entry->d_name)) continue;
		if (!strcmp("..",entry->d_name)) continue;

		/* read name */
		pmus[pmu_num].name=strdup(entry->d_name);
		sprintf(dir_name,SYSFS"/%s",
			entry->d_name);

		/* read type */
		sprintf(temp_name,"%s/type",dir_name);
		fff=fopen(temp_name,"r");
		if (fff==NULL) {
		}
		else {
			result=fscanf(fff,"%d",&type);
			if (result==1) pmus[pmu_num].type=type;
			fclose(fff);
		}

		/***********************/
		/* Scan format strings */
		/***********************/
		sprintf(format_name,"%s/format",dir_name);
		format_dir=opendir(format_name);
		if (format_dir==NULL) {
			/* Can be normal to have no format strings */
		}
		else {
			/* Count format strings */
			while(1) {
				format_entry=readdir(format_dir);
				if (format_entry==NULL) break;
				if (!strcmp(".",format_entry->d_name)) continue;
				if (!strcmp("..",format_entry->d_name)) continue;
				pmus[pmu_num].num_formats++;
			}

			/* Allocate format structure */
			pmus[pmu_num].formats=calloc(pmus[pmu_num].num_formats,
							sizeof(struct format_type));
			if (pmus[pmu_num].formats==NULL) {
				pmus[pmu_num].num_formats=0;
				closedir(dir);
				closedir(format_dir);
				return -1;
			}

			/* Read format string info */
			rewinddir(format_dir);
			format_num=0;
			while(1) {
				format_entry=readdir(format_dir);

				if (format_entry==NULL) break;
				if (!strcmp(".",format_entry->d_name)) continue;
				if (!strcmp("..",format_entry->d_name)) continue;

				pmus[pmu_num].formats[format_num].name=
					strdup(format_entry->d_name);
				sprintf(temp_name,"%s/format/%s",
					dir_name,format_entry->d_name);
				fff=fopen(temp_name,"r");
				if (fff!=NULL) {
					result=fscanf(fff,"%s",format_value);
					if (result==1) { 
						pmus[pmu_num].formats[format_num].value=
						strdup(format_value);
					}
					fclose(fff);

					parse_format(format_value,
						&pmus[pmu_num].formats[format_num].field,
						&pmus[pmu_num].formats[format_num].mask);
					format_num++;
				}
			}
			closedir(format_dir);
		}

		/***********************/
		/* Scan generic events */
		/***********************/
		sprintf(event_name,"%s/events",dir_name);
		event_dir=opendir(event_name);
		if (event_dir==NULL) {
			/* It's sometimes normal to have no generic events */
		}
		else {

			/* Count generic events */
			while(1) {
				event_entry=readdir(event_dir);
				if (event_entry==NULL) break;
				if (!strcmp(".",event_entry->d_name)) continue;
				if (!strcmp("..",event_entry->d_name)) continue;
				pmus[pmu_num].num_generic_events++;
			}

			/* Allocate generic events */
			pmus[pmu_num].generic_events=calloc(
				pmus[pmu_num].num_generic_events,
				sizeof(struct generic_event_type));
			if (pmus[pmu_num].generic_events==NULL) {
				pmus[pmu_num].num_generic_events=0;
				closedir(dir);
				closedir(event_dir);
				return -1;
			}

			/* Read in generic events */
			rewinddir(event_dir);
			generic_num=0;
			while(1) {
				event_entry=readdir(event_dir);
				if (event_entry==NULL) break;
				if (!strcmp(".",event_entry->d_name)) continue;
				if (!strcmp("..",event_entry->d_name)) continue;

				pmus[pmu_num].generic_events[generic_num].name=
					strdup(event_entry->d_name);
				sprintf(temp_name,"%s/events/%s",
					dir_name,event_entry->d_name);
				fff=fopen(temp_name,"r");
				if (fff!=NULL) {
					result=fscanf(fff,"%s",event_value);
					if (result==1) {
						pmus[pmu_num].generic_events[generic_num].value=
							strdup(event_value);
					}
					fclose(fff);
				}
				parse_generic(pmu_num,event_value,
						&pmus[pmu_num].generic_events[generic_num].config,
						&pmus[pmu_num].generic_events[generic_num].config1,
						&pmus[pmu_num].generic_events[generic_num].config2);
				generic_num++;
			}
			closedir(event_dir);
		}
		pmu_num++;
	}

	result = 0;

out:
	closedir(dir);

	return result;
}


static long long random_sysfs_config(__u32 *type,
				__u64 *config1,
				__u64 *config2) {

	int i,j;
	long long c=0,c1=0,c2=0;

	if (num_pmus==0) {
		/* For some reason we didn't get initialized */
		/* Fake it so we don't divide by zero        */
		*type=rand32();
		*config1=rand64();
		return rand64();
	}

	i=rand()%num_pmus;

	*type=pmus[i].type;

	switch(rand()%3) {
		/* Random by Format */
		case 0:
			if (pmus[i].num_formats==0) goto out;
			for(j=0;j<pmus[i].num_formats;j++) {
				/* 50% chance of having field set */
				if (rand()%2) {
					if (pmus[i].formats[j].field==FIELD_CONFIG) {
						c|=(rand64()&pmus[i].formats[j].mask);
					} else if (pmus[i].formats[j].field==FIELD_CONFIG1) {
						c1|=(rand64()&pmus[i].formats[j].mask);
					} else {
						c2|=(rand64()&pmus[i].formats[j].mask);
					}
				}
			}
			break;


		/* Random by generic event */
		case 1:
			if (pmus[i].num_generic_events==0) goto out;
			j=rand()%pmus[i].num_generic_events;
			c=pmus[i].generic_events[j].config;
			c1=pmus[i].generic_events[j].config1;
			c2=pmus[i].generic_events[j].config2;
			break;

		case 2:
			goto out;
			break;

		default:
			goto out;
			break;
	}
	*config1=c1;
	*config2=c2;
	return c;
out:
	*config1=rand()%64;
	return rand()%64;
}

/* arbitrary high number unlikely to be used by perf_event */
#define PERF_TYPE_READ_FROM_SYSFS 1027


static long long random_cache_config(void)
{

	int cache_id, hw_cache_op_id, hw_cache_op_result_id;

	switch (rand() % 8) {
	case 0:
		cache_id = PERF_COUNT_HW_CACHE_L1D;
		break;
	case 1:
		cache_id = PERF_COUNT_HW_CACHE_L1I;
		break;
	case 2:
		cache_id = PERF_COUNT_HW_CACHE_LL;
		break;
	case 3:
		cache_id = PERF_COUNT_HW_CACHE_DTLB;
		break;
	case 4:
		cache_id = PERF_COUNT_HW_CACHE_ITLB;
		break;
	case 5:
		cache_id = PERF_COUNT_HW_CACHE_BPU;
		break;
	case 6:
		cache_id = PERF_COUNT_HW_CACHE_NODE;
		break;
	case 7:
		cache_id = rand() % 256;
		break;
	default:
		cache_id = 0;
		break;
	}

	switch (rand() % 4) {
	case 0:
		hw_cache_op_id = PERF_COUNT_HW_CACHE_OP_READ;
		break;
	case 1:
		hw_cache_op_id = PERF_COUNT_HW_CACHE_OP_WRITE;
		break;
	case 2:
		hw_cache_op_id = PERF_COUNT_HW_CACHE_OP_PREFETCH;
		break;
	case 3:
		hw_cache_op_id = rand() % 256;
		break;
	default:
		hw_cache_op_id = 0;
		break;
	}

	switch (rand() % 3) {
	case 0:
		hw_cache_op_result_id = PERF_COUNT_HW_CACHE_RESULT_ACCESS;
		break;
	case 1:
		hw_cache_op_result_id = PERF_COUNT_HW_CACHE_RESULT_MISS;
		break;
	case 2:
		hw_cache_op_result_id = rand() % 256;
		break;
	default:
		hw_cache_op_result_id = 0;
		break;
	}

	return (cache_id) | (hw_cache_op_id << 8) | (hw_cache_op_result_id << 16);
}

static int random_event_type(void)
{

	int type=0;

	switch (rand() % 8) {
	case 0:
		type = PERF_TYPE_HARDWARE;
		break;
	case 1:
		type = PERF_TYPE_SOFTWARE;
		break;
	case 2:
		type = PERF_TYPE_TRACEPOINT;
		break;
	case 3:
		type = PERF_TYPE_HW_CACHE;
		break;
	case 4:
		type = PERF_TYPE_RAW;
		break;
	case 5:
		type = PERF_TYPE_BREAKPOINT;
		break;
	case 6:
		type = PERF_TYPE_READ_FROM_SYSFS;
		break;
	case 7:
		type = rand32();
		break;
	default:
		break;
	}
	return type;
}

static long long random_event_config(__u32 *event_type,
					__u64 *config1,
					__u64 *config2)
{
	unsigned long long config=0;

	switch (*event_type) {
	case PERF_TYPE_HARDWARE:
		switch (rand() % 11) {
		case 0:
			config = PERF_COUNT_HW_CPU_CYCLES;
			break;
		case 1:
			config = PERF_COUNT_HW_INSTRUCTIONS;
			break;
		case 2:
			config = PERF_COUNT_HW_CACHE_REFERENCES;
			break;
		case 3:
			config = PERF_COUNT_HW_CACHE_MISSES;
			break;
		case 4:
			config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
			break;
		case 5:
			config = PERF_COUNT_HW_BRANCH_MISSES;
			break;
		case 6:
			config = PERF_COUNT_HW_BUS_CYCLES;
			break;
		case 7:
			config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND;
			break;
		case 8:
			config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND;
			break;
		case 9:
			config = PERF_COUNT_HW_REF_CPU_CYCLES;
			break;
		case 10:
			config = rand64();
			break;
		default:
			break;
		}
		break;
	case PERF_TYPE_SOFTWARE:
		switch (rand() % 11) {
		case 0:
			config = PERF_COUNT_SW_CPU_CLOCK;
			break;
		case 1:
			config = PERF_COUNT_SW_TASK_CLOCK;
			break;
		case 2:
			config = PERF_COUNT_SW_PAGE_FAULTS;
			break;
		case 3:
			config = PERF_COUNT_SW_CONTEXT_SWITCHES;
			break;
		case 4:
			config = PERF_COUNT_SW_CPU_MIGRATIONS;
			break;
		case 5:
			config = PERF_COUNT_SW_PAGE_FAULTS_MIN;
			break;
		case 6:
			config = PERF_COUNT_SW_PAGE_FAULTS_MAJ;
			break;
		case 7:
			config = PERF_COUNT_SW_ALIGNMENT_FAULTS;
			break;
		case 8:
			config = PERF_COUNT_SW_EMULATION_FAULTS;
			break;
		case 9:
			config = PERF_COUNT_SW_DUMMY;
			break;
		case 10:
			config = rand64();
			break;
		default:
			break;
		}
		break;
	case PERF_TYPE_TRACEPOINT:
		/* Actual values to use can be found under */
		/* debugfs tracing/events/?*?/?*?/id       */
		/* usually a small < 4096 number           */
		switch(rand()%2) {
		case 0:
			/* Try a value < 4096 */
			config = rand()&0xfff;
			break;
		case 1:
			config = rand64();
			break;
		default:
			config = rand64();
			break;
		}
		break;
	case PERF_TYPE_HW_CACHE:
		config = random_cache_config();
		break;
	case PERF_TYPE_RAW:
		/* can be arbitrary 64-bit value */
		/* there are some constraints we can add */
		/* to make it more likely to be a valid event */
		config = rand64();
		break;
	case PERF_TYPE_BREAKPOINT:
		/* Breakpoint type only valid if config==0 */
		/* Set it to something else too anyway     */
		if (rand_bool())
			config = rand64();
		else
			config = 0;
		break;

	case PERF_TYPE_READ_FROM_SYSFS:
		config = random_sysfs_config(event_type,config1,config2);
		break;

	default:
		config = rand64();
		*config1 = rand64();
		*config2 = rand64();
		break;
	}
	return config;
}

static void setup_breakpoints(struct perf_event_attr *attr)
{

	switch (rand() % 6) {
	case 0:
		attr->bp_type = HW_BREAKPOINT_EMPTY;
		break;
	case 1:
		attr->bp_type = HW_BREAKPOINT_R;
		break;
	case 2:
		attr->bp_type = HW_BREAKPOINT_W;
		break;
	case 3:
		attr->bp_type = HW_BREAKPOINT_RW;
		break;
	case 4:
		attr->bp_type = HW_BREAKPOINT_X;
		break;
	case 5:
		attr->bp_type = rand32();
		break;
	default:
		break;
	}

	/* This might be more interesting if this were    */
	/* a valid executable address for HW_BREAKPOINT_X */
	/* or a valid mem location for R/W/RW             */
	attr->bp_addr = (long)get_address();

	switch (rand() % 5) {
	case 0:
		attr->bp_len = HW_BREAKPOINT_LEN_1;
		break;
	case 1:
		attr->bp_len = HW_BREAKPOINT_LEN_2;
		break;
	case 2:
		attr->bp_len = HW_BREAKPOINT_LEN_4;
		break;
	case 3:
		attr->bp_len = HW_BREAKPOINT_LEN_8;
		break;
	case 4:
		attr->bp_len = rand64();
		break;
	default:
		break;
	}
}

static long long random_sample_type(void)
{

	long long sample_type = 0;

	if (rand_bool())
		return rand64();

	if (rand_bool())
		sample_type |= PERF_SAMPLE_IP;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_TID;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_TIME;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_ADDR;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_READ;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_CALLCHAIN;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_ID;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_CPU;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_PERIOD;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_STREAM_ID;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_RAW;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_BRANCH_STACK;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_REGS_USER;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_STACK_USER;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_WEIGHT;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_DATA_SRC;
	if (rand_bool())
		sample_type |= PERF_SAMPLE_IDENTIFIER;

	return sample_type;
}

static long long random_read_format(void)
{

	long long read_format = 0;

	if (rand_bool())
		return rand64();

	if (rand_bool())
		read_format |= PERF_FORMAT_GROUP;
	if (rand_bool())
		read_format |= PERF_FORMAT_ID;
	if (rand_bool())
		read_format |= PERF_FORMAT_TOTAL_TIME_ENABLED;
	if (rand_bool())
		read_format |= PERF_FORMAT_TOTAL_TIME_RUNNING;

	return read_format;
}

static int random_attr_size(void) {

	int size=0;

	switch(rand() % 8) {
	case 0:	size = PERF_ATTR_SIZE_VER0;
		break;
	case 1: size = PERF_ATTR_SIZE_VER1;
		break;
	case 2: size = PERF_ATTR_SIZE_VER2;
		break;
	case 3: size = PERF_ATTR_SIZE_VER3;
		break;
	case 4: size = sizeof(struct perf_event_attr);
		break;
	case 5: size = rand32();
		break;
	case 6:	size = get_len();
		break;
	case 7: size = 0;
		break;
	default:
		break;
	}

	return size;
}

static long long random_branch_sample_type(void)
{

	long long branch_sample = 0;

	if (rand() % 2)
		return rand64();

	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_USER;
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_KERNEL;
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_HV;

	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_ANY;
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_ANY_CALL;
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_ANY_RETURN;
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_IND_CALL;

	/* Transactional Memory Types */
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_ABORT_TX;
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_IN_TX;
	if (rand_bool())
		branch_sample |= PERF_SAMPLE_BRANCH_NO_TX;

	return branch_sample;
}


static void create_mostly_valid_counting_event(struct perf_event_attr *attr,
						int group_leader)
{

	attr->type = random_event_type();
	attr->size = random_attr_size();
	attr->config = random_event_config(&attr->type,
					&attr->config1,
					&attr->config2);

	/* no freq for counting event */
	/* no sample type for counting event */

	attr->read_format = random_read_format();

	/* Bitfield parameters, mostly boolean */
	attr->disabled = rand_bool();
	attr->inherit = rand_bool();
	if (group_leader) {
		attr->pinned = rand_bool();
	}
	attr->exclusive = rand_bool();
	attr->exclude_user = rand_bool();
	attr->exclude_kernel = rand_bool();
	attr->exclude_hv = rand_bool();
	attr->exclude_idle = rand_bool();
	attr->mmap = rand_bool();
	attr->comm = rand_bool();
	attr->freq = rand_bool();
	attr->inherit_stat = rand_bool();
	attr->enable_on_exec = rand_bool();
	attr->task = rand_bool();
	attr->watermark = rand_bool();
	attr->precise_ip = rand() % 4;	// two bits
	attr->mmap_data = rand_bool();
	attr->sample_id_all = rand_bool();
	attr->exclude_host = rand_bool();
	attr->exclude_guest = rand_bool();
	attr->exclude_callchain_kernel = rand_bool();
	attr->exclude_callchain_user = rand_bool();

	/* wakeup events not relevant */

	/* breakpoint events unioned with config */
	if (attr->type == PERF_TYPE_BREAKPOINT) {
		setup_breakpoints(attr);
	} else {
		/* config1 set earlier */
		/* leave config2 alone for now */
	}

	/* branch_sample_type not relevant if not sampling */

	/* sample_regs_user not relevant if not sampling */

	/* sample_stack_user not relevant if not sampling */
}

static void create_mostly_valid_sampling_event(struct perf_event_attr *attr,
						int group_leader)
{

	attr->type = random_event_type();
	attr->size = random_attr_size();
	attr->config = random_event_config(&attr->type,
					&attr->config1,
					&attr->config2);

	/* low values more likely to have "interesting" results */
	attr->sample_period = rand64();
	attr->sample_type = random_sample_type();
	attr->read_format = random_read_format();

	/* Bitfield parameters, mostly boolean */
	attr->disabled = rand_bool();
	attr->inherit = rand_bool();
	/* only group leaders can be pinned */
	if (group_leader) {
		attr->pinned = rand_bool();
	} else {
		attr->pinned = 0;
	}
	attr->exclusive = rand_bool();
	attr->exclude_user = rand_bool();
	attr->exclude_kernel = rand_bool();
	attr->exclude_hv = rand_bool();
	attr->exclude_idle = rand_bool();
	attr->mmap = rand_bool();
	attr->comm = rand_bool();
	attr->freq = rand_bool();
	attr->inherit_stat = rand_bool();
	attr->enable_on_exec = rand_bool();
	attr->task = rand_bool();
	attr->watermark = rand_bool();
	attr->precise_ip = rand() % 4;	// two bits
	attr->mmap_data = rand_bool();
	attr->sample_id_all = rand_bool();
	attr->exclude_host = rand_bool();
	attr->exclude_guest = rand_bool();
	attr->exclude_callchain_kernel = rand_bool();
	attr->exclude_callchain_user = rand_bool();

	attr->wakeup_events = rand32();

	if (attr->type == PERF_TYPE_BREAKPOINT) {
		setup_breakpoints(attr);
	}
	else {
		/* breakpoint fields unioned with config fields */
		/* config1 set earlier */
	}

	attr->branch_sample_type = random_branch_sample_type();

	/* sample_regs_user is a bitmask of CPU registers to record.     */
	/* The values come from arch/ARCH/include/uapi/asm/perf_regs.h   */
	/* Most architectures have fewer than 64 registers...            */
	switch(rand()%3) {
		case 0:		attr->sample_regs_user = rand()%16;
				break;
		case 1:		attr->sample_regs_user = rand()%64;
				break;
		case 2:		attr->sample_regs_user = rand64();
				break;
		default:
				break;
	}

	/* sample_stack_user is the size of user stack backtrace we want  */
	/* if we pick too large of a value the kernel in theory truncates */
	attr->sample_stack_user = rand32();

}

/* Creates a completely random event, unlikely to be valid */
static void create_random_event(struct perf_event_attr *attr)
{

	attr->type = random_event_type();

	attr->size = random_attr_size();

	attr->config = random_event_config(&attr->type,
					&attr->config1,
					&attr->config2);

	attr->sample_period = rand64();
	attr->sample_type = random_sample_type();
	attr->read_format = random_read_format();

	/* bitfields */
	attr->disabled = rand_bool();
	attr->inherit = rand_bool();
	attr->pinned = rand_bool();
	attr->exclusive = rand_bool();
	attr->exclude_user = rand_bool();
	attr->exclude_kernel = rand_bool();
	attr->exclude_hv = rand_bool();
	attr->exclude_idle = rand_bool();
	attr->mmap = rand_bool();
	attr->comm = rand_bool();
	attr->freq = rand_bool();
	attr->inherit_stat = rand_bool();
	attr->enable_on_exec = rand_bool();
	attr->task = rand_bool();
	attr->watermark = rand_bool();
	attr->precise_ip = rand() % 4;
	attr->mmap_data = rand_bool();
	attr->sample_id_all = rand_bool();
	attr->exclude_host = rand_bool();
	attr->exclude_guest = rand_bool();
	attr->exclude_callchain_kernel = rand_bool();
	attr->exclude_callchain_user = rand_bool();

	attr->wakeup_events=rand32();

	/* Breakpoints are unioned with the config values */
	if (rand_bool()) {
		setup_breakpoints(attr);
	}
	else {
		/* config1 set earlier */
		attr->config2 = rand64();
	}

	attr->branch_sample_type = rand64();
	attr->sample_regs_user = rand64();
	attr->sample_stack_user = rand32();

}

void sanitise_perf_event_open(int childno)
{
	struct perf_event_attr *attr;
	unsigned long flags;
	pid_t pid;
	int group_leader=0;

	shm->a1[childno] = (unsigned long)page_rand;
	attr = (struct perf_event_attr *)shm->a1[childno];

	/* this makes sure we clear out the reserved fields. */
	memset(page_rand, 0, sizeof(struct perf_event_attr));

	/* cpu */
	/* requires ROOT to select specific CPU if pid==-1 (all processes) */
	/* -1 means all CPUs */

	switch(rand() % 2) {
	case 0:
		/* Any CPU */
		shm->a3[childno] = -1;
		break;
	case 1:
		/* Default to the get_cpu() value */
		/* set by ARG_CPU                 */
	default:
		break;
	}

	/* group_fd */
	/* should usually be -1 or another perf_event fd         */
	/* Anything but -1 unlikely to work unless the other pid */
	/* was properly set up to be a group master              */
	switch (rand() % 3) {
	case 0:
		shm->a4[childno] = -1;
		group_leader = 1;
		break;
	case 1:
		/* Try to get a previous random perf_event_open() fd  */
		/* It's unclear whether get_random_fd() would do this */
		shm->a4[childno] = rand() % 1024;
		break;
	case 2:
		/* Rely on ARG_FD */
		break;
	default:
		break;
	}

	/* flags */
	/* You almost never set these unless you're playing with cgroups */
	flags = 0;
	if (rand_bool()) {
		flags = rand64();
	} else {
		if (rand_bool())
			flags |= PERF_FLAG_FD_NO_GROUP;
		if (rand_bool())
			flags |= PERF_FLAG_FD_OUTPUT;
		if (rand_bool())
			flags |= PERF_FLAG_PID_CGROUP;
	}
	shm->a5[childno] = flags;

	/* pid */
	/* requires ROOT to select pid that doesn't belong to us */

	if (flags & PERF_FLAG_PID_CGROUP) {
		/* In theory in this case we should pass in */
		/* a file descriptor from /dev/cgroup       */
		pid = get_random_fd();
	} else {
		switch(rand() % 4) {
		case 0:	/* use current thread */
			pid = 0;
			break;
		case 1: /* get an arbitrary pid */
			pid = get_pid();
			break;
		case 2:	/* measure *all* pids.  Might require root */
			pid = -1;
			break;
		case 3: /* measure our actual pid */
			pid=getpid();
			break;
		default:
			pid = 0;
			break;
		}
	}
	shm->a2[childno] = pid;

	/* set up attr structure */
	switch (rand() % 3) {
	case 0:
		create_mostly_valid_counting_event(attr,group_leader);
		break;
	case 1:
		create_mostly_valid_sampling_event(attr,group_leader);
		break;
	case 2:
		create_random_event(attr);
		break;
	default:
		break;
	}
}

struct syscall syscall_perf_event_open = {
	.name = "perf_event_open",
	.num_args = 5,
	.arg1name = "attr_uptr",
	.arg1type = ARG_ADDRESS,
	.arg2name = "pid",
	.arg2type = ARG_PID,
	.arg3name = "cpu",
	.arg3type = ARG_CPU,
	.arg4name = "group_fd",
	.arg4type = ARG_FD,
	.arg5name = "flags",
	.arg5type = ARG_LIST,
	.arg5list = {
		.num = 3,
		.values = {
			PERF_FLAG_FD_NO_GROUP, PERF_FLAG_FD_OUTPUT, PERF_FLAG_PID_CGROUP
		},
	},
	.sanitise = sanitise_perf_event_open,
	.init = init_pmus,
	.flags = NEED_ALARM,
};
