#ifndef _MAPS_H
#define _MAPS_H 1

void generate_random_page(char *page);

extern char *page_zeros;
extern char *page_0xff;
extern char *page_rand;
extern char *page_allocs;

struct map {
	struct map *next;
	void *ptr;
	char *name;
	unsigned long size;
};
void setup_maps(void);
void destroy_maps(void);
void * get_map(void);
void init_buffers(void);

#endif	/* _MAPS_H */
