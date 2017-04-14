#include "constants.h"
#include <sys/stat.h>

void setup_fds(void);

void generate_filelist(void);
void open_files(void);
void close_files(void);
void regenerate_fds(void);

void parse_devices(void);
const char *map_dev(dev_t, mode_t);

extern unsigned int nr_file_fds;
extern char *victim_path;
extern const char **fileindex;
extern unsigned int files_in_index;
