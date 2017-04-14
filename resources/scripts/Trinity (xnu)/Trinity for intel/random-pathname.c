#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arch.h"
#include "log.h"
#include "maps.h"
#include "random.h"
#include "sanitise.h"

const char * generate_pathname(void)
{
	const char *pathname = get_filename();
	char *newpath;
	unsigned int len;
	unsigned int i;

	if (pathname == NULL)		/* handle -n correctly. */
		return NULL;

	/* 90% chance of returning an unmangled filename */
	if ((rand() % 100) < 90)
		return pathname;

	/* Create a bogus filename. */
	newpath = malloc(page_size);	// FIXME: We leak this.
	if (newpath == NULL)
		return pathname;	// give up.

	len = strlen(pathname);

	/* empty string. */
	if ((rand() % 100) == 0) {
		memset(newpath, 0, page_size);
		goto out;
	}

	generate_random_page(newpath);

	/* sometimes, just complete junk. */
	if (rand_bool())
		goto out;

	/* Sometimes, pathname + junk */
	if (rand_bool())
		(void) strncpy(newpath, pathname, len);
	else {
		/* make it look relative to cwd */
		newpath[0] = '.';
		newpath[1] = '/';
		(void) strncpy(newpath + 2, pathname, len);
	}

	/* Sometimes, remove all /'s */
	if (rand_bool()) {
		for (i = 0; i < len; i++) {
			if (newpath[i] == '/')
				newpath[i] = rand();
		}
	}
out:
	/* 50/50 chance of making it look like a dir */
	if (rand_bool()) {
		newpath[len] = '/';
		newpath[len + 1] = 0;
	}

	return newpath;
}
