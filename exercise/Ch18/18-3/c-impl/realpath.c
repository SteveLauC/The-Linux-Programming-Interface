#include "realpath.h"

char *my_realpath(const char *path, char *resolved_path)
{
	if (path == NULL) {
		errno = EINVAL;
		return NULL;
	}

	char *new_path = remove_extra_slash(path);
	struct Paths p;
	char *cwd = getcwd(NULL, 0);
	paths_init(&p, is_absolute(new_path) ? NULL : cwd, new_path);
	free(cwd);
	free(new_path);

	char *next_entry = NULL;
	while ((next_entry = remained_next_entry(&p)) != NULL) {
		if (is_dot(next_entry)) {
			// deallocate memory before we skip this loop
			free(next_entry);
			continue;
		} else if (is_a_pair_of_dot(next_entry)) {
			parsed_cd_to_parent(&p);
		} else {
			parsed_push_back(&p, next_entry);
		}
		free(next_entry);

		//				 UNCOMMNT if you wanna dereference symlink
		//		struct stat stat_buf;
		//		// make it a string
		//		p.parsed[p.parsed_len] = '\0';
		//		if (lstat(p.parsed, &stat_buf) == -1) {
		//			return NULL;
		//		}
		//
		//		if (S_ISLNK(stat_buf.st_mode)) {
		//			// 1 extra byte for NUL
		//			char link_content[stat_buf.st_size + 1];
		//			int link_content_len = readlink(p.parsed, link_content,
		//							stat_buf.st_size + 1);
		//			// add the tailing NUL
		//			link_content[link_content_len] = '\0';
		//			char *link = remove_extra_slash(link_content);
		//
		//			if (is_absolute(link)) {
		//				replace_parsed(&p, link);
		//			} else {
		//				parsed_cd_to_parent(&p);
		//				remained_push_front(&p, link);
		//			}
		//			free(link);
		//		}
	}

	if (resolved_path == NULL) {
		char *ret = malloc(p.parsed_len + 1);
		if (ret == NULL) {
			fprintf(stderr, "fatal error: can not allocate memory");
			exit(1);
		}
		memcpy(ret, p.parsed, p.parsed_len);
		ret[p.parsed_len] = '\0';
		return ret;
	} else {
		memcpy(resolved_path, p.parsed, p.parsed_len);
		resolved_path[p.parsed_len] = '\0';
		return resolved_path;
	}
}
