#include "utility.h"
#include "realpath.h"

void test_utility(void);
void test_is_absolute(void);
void test_replaced_parsed(void);
void test_remained_next_entry(void);
void test_remained_push_front(void);
void test_remove_extra_slash(void);
void test_parsed_push_back(void);
void test_parsed_cd_to_parent(void);
void test_realpath(void);

void test_is_absolute(void)
{
	assert(is_absolute("/") == 1);
	assert(is_absolute("/home") == 1);
	assert(is_absolute("./") == 0);
	assert(is_absolute("../") == 0);
	assert(is_absolute("home") == 0);
}

void test_replaced_parsed(void)
{
	char *parsed = "/home/steve";

	struct Paths p;
	paths_init(&p, parsed, NULL);

	char *new_parsed = "/usr/bin";
	int len_of_new_parsed = strlen(new_parsed);
	replace_parsed(&p, new_parsed);

	assert(strncmp(p.parsed, new_parsed, strlen(new_parsed)) == 0);
	assert(len_of_new_parsed == p.parsed_len);
}

void test_remained_next_entry(void)
{
	char *remained1 = "/home";
	struct Paths p1;

	paths_init(&p1, NULL, remained1);

	char *ret1 = remained_next_entry(&p1);
	assert(ret1 != NULL);
	assert(is_slash(ret1));
	free(ret1);
	assert(p1.remained_start == 1);

	char *remained2 = "steve/Desktop";

	struct Paths p2;
	paths_init(&p2, NULL, remained2);

	char *ret2 = remained_next_entry(&p2);
	assert(ret2 != NULL);
	assert(strncmp("steve", ret2, strlen(ret2)) == 0);
	free(ret2);

	char *ret3 = remained_next_entry(&p2);
	assert(ret3 != NULL);
	assert(strncmp("Desktop", ret3, strlen(ret3)) == 0);
	free(ret3);

	char *ret4 = remained_next_entry(&p2);
	assert(ret4 == NULL);
}

void test_remained_push_front(void)
{
	char *remained = "home";
	struct Paths p;

	paths_init(&p, NULL, remained);

	remained_push_front(&p, ".////file/");
	assert(strncmp(p.remained, "./file/home", strlen("./file/home")) == 0);
}

void test_remove_extra_slash(void)
{
	char *path1 = "///home///steve///";
	char *res1 = remove_extra_slash(path1);
	assert(res1 != NULL);
	assert((strncmp(res1, "/home/steve", strlen(res1))) == 0);
	free(res1);

	char *path2 = "/";
	char *res2 = remove_extra_slash(path2);
	assert(res2 != NULL);
	assert((strncmp(res2, "/", strlen(res2))) == 0);
	free(res2);
}

void test_parsed_push_back(void)
{
	char *entry = "steve";
	char *parsed = "/home";
	int len_of_entry = strlen(entry);
	int len_of_parsed = strlen(parsed);
	struct Paths p;
	paths_init(&p, parsed, NULL);
	parsed_push_back(&p, entry);
	assert(strncmp(p.parsed, "/home/steve", strlen("/home/steve")) == 0);
	assert(p.parsed_len == len_of_parsed + len_of_entry + 1);

	struct Paths p2;
	paths_init(&p2, NULL, NULL);
	parsed_push_back(&p2, "/");
	assert(strncmp(p2.parsed, "/", strlen("/")) == 0);
	assert(p2.parsed_len == 1);
}

void test_parsed_cd_to_parent(void)
{
	struct Paths p1;
	paths_init(&p1, "/", NULL);
	parsed_cd_to_parent(&p1);
	assert(p1.parsed_len == 1);

	struct Paths p2;
	paths_init(&p2, "/home", NULL);
	parsed_cd_to_parent(&p2);
	assert(p2.parsed_len == 1);

	struct Paths p3;
	paths_init(&p3, "/home/steve", NULL);
	parsed_cd_to_parent(&p3);
	assert(p3.parsed_len == 5);

	struct Paths p4;
	paths_init(
		&p4,
		"/home/steve/Documents/workspace/The-Linux-Programming-Interface/exercise/Ch18/18-3/c-impl/test",
		NULL);
	parsed_cd_to_parent(&p4);
	char *new_parsed =
		"/home/steve/Documents/workspace/The-Linux-Programming-Interface/exercise/Ch18/18-3/c-impl";
	assert(strncmp(p4.parsed, new_parsed, strlen(new_parsed)) == 0);
	assert(p4.parsed_len == (int)strlen(new_parsed));
}

void test_utility(void)
{
	test_is_absolute();
	test_replaced_parsed();
	test_remained_next_entry();
	test_remained_push_front();
	test_remove_extra_slash();
	test_parsed_push_back();
	test_parsed_cd_to_parent();
	printf("All utility tests passed!\n");
}

void test_realpath(void)
{
	char *cwd = getcwd(NULL, 0);
	int len_of_cwd = strlen(cwd);
	char buf[PATH_MAX];
	memcpy(buf, cwd, len_of_cwd);

	char *res1 = my_realpath("/..", NULL);
	assert(strncmp(res1, "/", strlen(res1)) == 0);
	free(res1);

	char *res2 = my_realpath("/../test", NULL);
	assert(strncmp(res2, "/test", strlen(res2)) == 0);
	free(res2);

	char *res3 = my_realpath("test/..", NULL);
	assert(strncmp(cwd, res3, strlen(res3)) == 0);
	free(res3);

	char *res4 = my_realpath("test/path/..", NULL);
	memcpy(buf + len_of_cwd, "/", 1);
	memcpy(buf + len_of_cwd + 1, "test", strlen("test"));
	buf[len_of_cwd + strlen("test") + 1] = '\0';
	assert(strncmp(buf, res4, strlen(res4)) == 0);
	free(res4);

	char *res5 = my_realpath("test/../path", NULL);
	memcpy(buf + len_of_cwd, "/", 1);
	memcpy(buf + len_of_cwd + 1, "path", strlen("path"));
	buf[len_of_cwd + strlen("path") + 1] = '\0';
	assert(strncmp(buf, res5, strlen(res5)) == 0);
	free(res5);

	char *res6 = my_realpath("test/path/../../", NULL);
	assert(strncmp(cwd, res6, strlen(res6)) == 0);
	free(res6);

	free(cwd);

	printf("test_my_realpath passed!\n");
}