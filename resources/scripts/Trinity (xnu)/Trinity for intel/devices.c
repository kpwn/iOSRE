/*
 * Routines for parsing /proc/devices for use by the ioctl fuzzer.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <linux/kdev_t.h>
#include "files.h"

#include "log.h"

#define MAJOR(x) x
#define MINOR(x) x

static struct {
	int major;
	int minor;
	const char *name;
} *block_devs, *char_devs, *misc_devs;

static size_t bldevs, chrdevs, miscdevs;

static void parse_proc_devices(void)
{
	FILE *fp;
	char *p, *name, *line = NULL;
	size_t n = 0;
	int block, major;
	void *new;

	fp = fopen("/proc/devices", "r");
	if (!fp)
		return;

	block = 0;

	while (getline(&line, &n, fp) >= 0) {
		if (strcmp("Block devices:\n", line) == 0)
			block = 1;
		else if (strcmp("Character devices:\n", line) == 0)
			block = 0;
		else if (sscanf(line, "%d %*s", &major) == 1) {
			if ((p = strchr(line, '\n')) != NULL)
				*p = 0;
			if ((p = strrchr(line, ' ')) == NULL)
				continue;
			p++;
			name = strdup(p);

			if (block) {
				new = realloc(block_devs, (bldevs+1)*sizeof(*block_devs));
				if (!new) {	// FIXME: We should propagate failure up here.
					free(name);
					break;
				}
				block_devs = new;
				block_devs[bldevs].major = major;
				block_devs[bldevs].minor = 0;
				block_devs[bldevs].name = name;
				bldevs++;
			} else {
				new = realloc(char_devs, (chrdevs+1)*sizeof(*char_devs));
				if (!new) {
					free(name);
					break;
				}
				char_devs = new;
				char_devs[chrdevs].major = major;
				char_devs[chrdevs].minor = 0;
				char_devs[chrdevs].name = name;
				chrdevs++;
			}
		}
	}

	fclose(fp);
	free(line);
}

static void parse_proc_misc(void)
{
	FILE *fp;
	char *name;
	int minor;
	void *new;

	fp = fopen("/proc/misc", "r");
	if (!fp)
		return;

	while (fscanf(fp, "%d %as", &minor, &name) == 2) {
		new = realloc(misc_devs, (miscdevs+1)*sizeof(*misc_devs));
		if (!new) {
			free(name);
			break;
		}
		misc_devs = new;
		misc_devs[miscdevs].major = 0;
		misc_devs[miscdevs].minor = minor;
		misc_devs[miscdevs].name = name;
		miscdevs++;
	}

	fclose(fp);
}

void parse_devices(void)
{
	parse_proc_devices();
	parse_proc_misc();

	output(2, "Parsed %zu char devices, %zu block devices, %zu misc devices.\n",
			chrdevs, bldevs, miscdevs);
}

const char *map_dev(dev_t st_rdev, mode_t st_mode)
{
	int major, minor;
	size_t i, j;

	major = MAJOR(st_rdev);
	minor = MINOR(st_rdev);

	if (S_ISCHR(st_mode)) {
		for (i = 0; i < chrdevs; ++i) {
			if (char_devs[i].major == major) {
				if (strcmp(char_devs[i].name, "misc") == 0) {
					for (j=0; j < miscdevs; ++j)
						if (misc_devs[j].minor == minor)
							return misc_devs[j].name;
				} else
					return char_devs[i].name;
			}
		}
	} else if (S_ISBLK(st_mode)) {
		for (i = 0; i < bldevs; ++i) {
			if (block_devs[i].major == major)
				return block_devs[i].name;
		}
	}

	return NULL;
}
