#include "utility.h"

void paths_init(struct Paths *p, const char *parsed, const char *remained)
{
	assert(p != NULL);

	// initialized `parsed` and `remained` to 0s though it is fine without
	// initialization, but to silent valgrind, we have to do this.
	memset(p->parsed, 0, PATH_MAX);
	memset(p->remained, 0, PATH_MAX);

	if (parsed == NULL) {
		p->parsed_len = 0;
	} else {
		int len_parsed = strlen(parsed);
		memcpy(p->parsed, parsed, len_parsed);
		p->parsed_len = len_parsed;
	}

	p->remained_start = 0;
	if (remained == NULL) {
		p->remained_after_end = 0;
	} else {
		int len_remained = strlen(remained);
		memcpy(p->remained, remained, len_remained);
		p->remained_after_end = len_remained;
	}
}

// Return `1` is `path` is a absolute path. Otherwise, return `0`.
inline int is_absolute(const char *path)
{
	assert(path != NULL);
	return path[0] == '/';
}

// Replace `paths_p->parsed` with `path`
void replace_parsed(struct Paths *paths_p, const char *path)
{
	assert(paths_p != NULL && path != NULL);

	int len = (int)strlen(path);
	paths_p->parsed_len = len;

	// Copy to `parsed`, excluding the tailing NUL
	memcpy(paths_p->parsed, path, len);
}

// Return next entry from `paths_p->remained`, and adjust `paths_p->remained_start`
// Return `NULL` when there is no more entries. The return entry is heap-allocated,
// caller should manually deallocate it.
//
// There are four typical cases:
// 1. "" -> NULL
// 2. "/home" -> "/"
// 3. "steve/Desktop" -> "steve"
// 4. "Desktop" -> "Desktop"
char *remained_next_entry(struct Paths *paths_p)
{
	assert(paths_p != NULL);

	int start = paths_p->remained_start;
	int end = paths_p->remained_after_end;
	int has_entry = (start != end);

	if (!has_entry) {
		return NULL;
	} else if (is_absolute(paths_p->remained + start)) {
		assert(start == 0);
		paths_p->remained_start += 1;
		char *ret = malloc(2);
		ret[0] = '/';
		ret[1] = '\0';
		return ret;
	} else {
		int idx = paths_p->remained_start;

		while (idx != end) {
			if (paths_p->remained[idx] == '/') {
				break;
			}
			idx += 1;
		}

		int next_entry_len = idx - start;
		char *ret = malloc(next_entry_len + 1); // 1 extra byte for NUL
		memcpy(ret, (paths_p->remained + start), next_entry_len);
		ret[next_entry_len] = '\0'; // add a tailing NUL

		if (paths_p->remained[idx] == '/') {
			paths_p->remained_start = idx + 1;
		} else {
			paths_p->remained_start = end;
		}

		return ret;
	}
}

// Push `entry` to the front of `paths_p->remained`, and update `remained_start`
// and `remained_end`
//
// Used for pushing the contents (a *relative* path) of a symlink to the front
// of `remained`
void remained_push_front(struct Paths *paths_p, const char *entry)
{
	assert(paths_p != NULL && entry != NULL);

	// filter it first, since the contents of a symlink can be messy
	char *entry_after_filter = remove_extra_slash(entry);
	int len_of_entry = strlen(entry_after_filter);

	int len_of_remained =
		paths_p->remained_after_end - paths_p->remained_start;

	// 1 extra byte for the middle "/", excluding NUL
	int total_len = len_of_entry + len_of_remained + 1;

	char buf[total_len];
	int cursor_of_buf = 0;
	// concatenation
	memcpy(buf + cursor_of_buf, entry_after_filter, len_of_entry);
	cursor_of_buf += len_of_entry;
	memcpy(buf + cursor_of_buf, "/", 1);
	cursor_of_buf += 1;
	memcpy(buf + cursor_of_buf,
	       (paths_p->remained + paths_p->remained_start), len_of_remained);

	// copy to remained
	memcpy(paths_p->remained, buf, total_len);
	// update cursors
	paths_p->remained_start = 0;
	paths_p->remained_after_end = total_len;

	free(entry_after_filter);
}

// Return `1` is `path` is "/". Otherwise, return `0`.
inline int is_slash(const char *path)
{
	assert(path != NULL);
	return (strncmp(path, "/", strlen(path)) == 0);
}

inline int is_dot(const char *str)
{
	assert(str != NULL);
	return (strncmp(str, ".", strlen(str)) == 0);
}

inline int is_a_pair_of_dot(const char *str)
{
	assert(str != NULL);
	return (strncmp(str, "..", strlen(str)) == 0);
}

static int last_char_in_buf_is_slash(const char *buf, int cursor_of_buf)
{
	assert(buf != NULL && cursor_of_buf >= 0);

	if (cursor_of_buf == 0) {
		return 0;
	}

	return (buf[cursor_of_buf - 1] == '/');
}

// Replace multiple, continuous `/` with a single one, and remove the ending
// `/` if `path` is not `/` and has a tailing slash.
//
// The path returned is heap-allocated, use `free(3)` to deallocate it.
//
// Examples:
// "/" -> "/"
// "///home/" -> "/home"
char *remove_extra_slash(const char *path)
{
	assert(path != NULL);

	int len_of_path = strlen(path);
	// 1 extra byte for NUL
	char *buf = malloc(len_of_path + 1);
	int cursor_of_buf = 0;
	int cursor_of_path = 0;

	while (cursor_of_path < len_of_path) {
		if (path[cursor_of_path] == '/' &&
		    last_char_in_buf_is_slash(buf, cursor_of_buf)) {
			cursor_of_path += 1;
			continue;
		}

		// copy
		buf[cursor_of_buf] = path[cursor_of_path];
		cursor_of_buf += 1;
		cursor_of_path += 1;
	}

	// When exiting the loop, `cursor_of_buf` is the index of the last char
	// + 1.

	int len_buf = cursor_of_buf;

	if (len_buf != 1 && buf[cursor_of_buf - 1] == '/') {
		buf[cursor_of_buf - 1] = '\0';
	} else {
		buf[cursor_of_buf] = '\0';
	}

	return buf;
}

// Push `entry` to the `parsed`. `parsed` may be uninitialized.
//
// `entry` can be:
// 1. "/"
// 2. "file"
//
// `parsed` can be:
// 1. emtpy (parsed_len = 0)
// 2. "/" (parsed_len = 1)
// 3. "/home" (parsed_len > 1)
//
// When `parsed` is empty (i.e., `parsed_len` equals to 0), in our usage,
// `entry` is guaranteed to be "/". If `parsed` is not empty, then we can
// need to determine whether we need to push a slash first.
void parsed_push_back(struct Paths *paths_p, const char *entry)
{
	assert(paths_p != NULL && entry != NULL);
	int len_of_entry = strlen(entry);

	// parsed is empty
	if (paths_p->parsed_len == 0) {
		assert(is_slash(entry));
		memcpy((paths_p->parsed + paths_p->parsed_len), "/", 1);
		paths_p->parsed_len += 1;
	} else if (paths_p->parsed_len == 1) {
		// parsed is "/"
		memcpy((paths_p->parsed + paths_p->parsed_len), entry, len_of_entry);
		paths_p->parsed_len += len_of_entry;
	} else {
		// parsed is something like "/home/steve", you need to push a
		// slash plus entry
		memcpy((paths_p->parsed + paths_p->parsed_len), "/", 1);
		paths_p->parsed_len += 1;
		memcpy((paths_p->parsed + paths_p->parsed_len), entry,
		       len_of_entry);
		paths_p->parsed_len += len_of_entry;
	}
}

// Change `parsed` to its parent directory, and update `parsed_end`
//
// Note that `parsed` won't have a tailing "/", we will use this feature in the
// impl.
//
// "/" -> "/"
// "/home" -> "/"
// "/home/steve" -> "/home"
void parsed_cd_to_parent(struct Paths *paths_p)
{
	// "/"
	if (paths_p->parsed[paths_p->parsed_len - 1] == '/') {
		return;
	}

	int idx = paths_p->parsed_len - 1;
	while (paths_p->parsed[idx] != '/') {
		idx -= 1;
	}
	assert(paths_p->parsed[idx] == '/');

	// parsed is something like "/" now
	if (idx == 0) {
		idx += 1;
	}
	paths_p->parsed_len = idx;
}
