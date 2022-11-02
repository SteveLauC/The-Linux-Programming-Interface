#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <ftw.h>

#define MAX_NUM_ENTRY 100

int iterate_over_dir(const char *, const struct stat *, int, struct FTW *);
int print_and_watch_a_file_tree(const char *, const struct stat *, int,
				struct FTW *);

typedef struct {
	int wd;
	char watched_file[PATH_MAX];
	int is_root;
} Entry;

Entry *entry_new(int, const char *, int);
void entry_dbg(const Entry *self);

typedef struct {
	Entry *data[MAX_NUM_ENTRY];
	int len;
} Mappings;

void mappings_init(Mappings *);
int mappings_is_root(const Mappings *, int);
void mappings_add(Mappings *, Entry *);
Entry *mappings_try_to_remove(Mappings *, int);
char *mappings_get_watched_file_path(const Mappings *, int);
void mappings_clear(const Mappings *);
void mappings_dbg(const Mappings *);

void handle_event(const struct inotify_event *, int, Mappings *);

extern int inotify;
extern Mappings mappings;
extern uint32_t mask;
