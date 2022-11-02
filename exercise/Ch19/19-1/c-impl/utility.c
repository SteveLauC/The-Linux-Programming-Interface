#define _GNU_SOURCE

#include "utility.h"
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>

int iterate_over_dir(const char *fpath, const struct stat *sb, int typeflag,
		     struct FTW *ftwbuf)
{
	if (typeflag == FTW_NS) {
		return -1;
	}

	if (S_ISDIR(sb->st_mode)) {
		int wd = -1;
		if ((wd = inotify_add_watch(inotify, fpath, mask)) == -1) {
			fprintf(stderr, "inotify_add_watch(2)");
			exit(EXIT_FAILURE);
		}
		mappings_add(&mappings, entry_new(wd, fpath, 0));
	}

	return 0;
}


// print all the files inside the file tree and watch all its directories.
//
// Used in `handle_event` `IN_CREATE` branch.
int print_and_watch_a_file_tree(const char *fpath, const struct stat *sb, int typeflag,
		     struct FTW *ftwbuf)
{
	if (typeflag == FTW_NS) {
		return -1;
	}

	printf("%s was created\n", fpath);

	if (S_ISDIR(sb->st_mode)) {
		int wd = -1;
		if ((wd = inotify_add_watch(inotify, fpath, mask)) == -1) {
			fprintf(stderr, "inotify_add_watch(2)");
			exit(EXIT_FAILURE);
		}
		mappings_add(&mappings, entry_new(wd, fpath, 0));
	}

	return 0;
}


// Return a heap-allocated `Entry`
//
// When on error, NULL is returned.
Entry *entry_new(int wd, const char *watched_file_path, int is_root)
{
	assert(watched_file_path != NULL);

	Entry *entry = malloc(sizeof(Entry));
	if (entry == NULL) {
		return NULL;
	}
	memset(entry, 0, sizeof(Entry));
	entry->wd = wd;
	strncpy(entry->watched_file, watched_file_path,
		strlen(watched_file_path) + 1);
	entry->is_root = is_root;

	return entry;
}

void entry_dbg(const Entry *self)
{
	printf("wd: %d watched_file: %s is_root: %d\n", self->wd,
	       self->watched_file, self->is_root);
}

// Initialize the `mapping` entry
void mappings_init(Mappings *self)
{
	assert(self != NULL);
	self->len = 0;
	memset(self->data, 0, MAX_NUM_ENTRY * sizeof(struct Entry *));
}

int mappings_is_root(const Mappings *self, int wd)
{
	assert(self != NULL);
	for (int i = 0; i < self->len; i += 1) {
		if (self->data[i]->wd == wd) {
			return self->data[i]->is_root;
		}
	}

	fprintf(stderr,
		"fatal error: The wd to be queries does not exist in the mapping");
	exit(EXIT_FAILURE);
}

void mappings_add(Mappings *self, Entry *entry)
{
	assert(self != NULL && entry != NULL);
	self->data[self->len] = entry;
	self->len += 1;
}

// Try to remove the `Entry` whose watch descriptor is `wd` from the mappings.
//
// If it exists, remove it and return its address. Otherwise, return NULL.
Entry *mappings_try_to_remove(Mappings *self, int wd)
{
	assert(self != NULL);
	int idx = -1;
	for (int i = 0; i < self->len; i += 1) {
		if (self->data[i]->wd == wd) {
			idx = i;
			break;
		}
	}

	// swap remove the item with index `idx`
	if (idx != -1) {
		Entry *temp = self->data[idx];
		self->data[idx] = self->data[self->len - 1];
		self->data[self->len - 1] = temp;

		self->len -= 1;

		return self->data[self->len];
	}

	return NULL;
}

// Get the `watched_file` field of the `Entry` whose watch descriptor is `wd`
//
// The return value is simply the starting address of that string.
char *mappings_get_watched_file_path(const Mappings *self, int wd)
{
	assert(self != NULL);
	int idx = -1;
	for (int i = 0; i < self->len; i += 1) {
		if (self->data[i]->wd == wd) {
			idx = i;
			break;
		}
	}

	if (idx == -1) {
		return NULL;
	} else {
		return self->data[idx]->watched_file;
	}
}

// clear all the `Entry` in the map
void mappings_clear(const Mappings *self)
{
	for (int i = 0; i < self->len; i += 1) {
		free(self->data[i]);
	}
}

void mappings_dbg(const Mappings *self)
{
	printf("DEBUGING mappings...\n");
	printf("LEN=%d\n", self->len);
	for (int i = 0; i < self->len; i += 1) {
		entry_dbg(self->data[i]);
	}
}

void handle_event(const struct inotify_event *event, int inotify,
		  Mappings *mappings)
{
	uint32_t returned_mask = event->mask;
	int wd = event->wd;

	if (returned_mask & IN_CREATE) {
		char *watched_directory =
			mappings_get_watched_file_path(mappings, wd);
		assert(watched_directory != NULL);
		size_t len_of_watched_directory = strlen(watched_directory);
		char path[PATH_MAX] = { 0 };
		if (event->len != 0) {
			memcpy(path, watched_directory,
			       len_of_watched_directory);
			memcpy(path + len_of_watched_directory, "/", 1);
			strncpy(path + len_of_watched_directory + 1,
				event->name, strlen(event->name) + 1);
		}

		nftw(path, print_and_watch_a_file_tree, 20, 0);
	}

	if (returned_mask & IN_DELETE) {
		printf("%s was deleted\n", event->name);
	}

	if (returned_mask & IN_DELETE_SELF) {
		char *watched_directory =
			mappings_get_watched_file_path(mappings, wd);
		assert(watched_directory != NULL);

		if (mappings_is_root(mappings, wd)) {
			printf("Root %s was deleted\n", watched_directory);
			mappings_clear(mappings);
			exit(EXIT_SUCCESS);
		} else {
			Entry *removed_entry =
				mappings_try_to_remove(mappings, wd);
			if (removed_entry != NULL) {
				free(removed_entry);
			}
		}
	}

	if (returned_mask & IN_MOVE_SELF) {
		char *watched_directory =
			mappings_get_watched_file_path(mappings, wd);
		assert(watched_directory != NULL);

		if (mappings_is_root(mappings, wd)) {
			printf("Root %s was moved\n", watched_directory);
			// Root is gone, exit the program.
			mappings_clear(mappings);
			exit(EXIT_SUCCESS);
		} else {
			if (inotify_rm_watch(inotify, wd) == -1) {
				mappings_clear(mappings);
				perror("inotify_rm_watch(2)");
				exit(EXIT_FAILURE);
			}

			Entry *removed_entry =
				mappings_try_to_remove(mappings, wd);
			if (removed_entry != NULL) {
				free(removed_entry);
			}
		}
	}

	if (returned_mask & IN_MOVED_FROM) {
		printf("%s was moved\n", event->name);
	}

	if (returned_mask & IN_MOVED_TO) {
		char *watched_directory =
			mappings_get_watched_file_path(mappings, wd);
		assert(watched_directory != NULL);

		size_t len_of_watched_directory = strlen(watched_directory);
		char path[PATH_MAX] = { 0 };
		if (event->len != 0) {
			memcpy(path, watched_directory,
			       len_of_watched_directory);
			memcpy(path + len_of_watched_directory, "/", 1);
			strncpy(path + len_of_watched_directory + 1,
				event->name, strlen(event->name) + 1);
		}

		printf("%s was moved to %s\n", event->name, watched_directory);

		struct stat stat_buf;
		if (lstat(path, &stat_buf) == -1) {
			mappings_clear(mappings);
			perror("lstat(2)");
			exit(EXIT_FAILURE);
		}

		if (S_ISDIR(stat_buf.st_mode)) {
			nftw(path, iterate_over_dir, 20, 0);
		}
	}
}
