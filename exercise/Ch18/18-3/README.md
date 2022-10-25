#### Question

Implement `realpath()`

#### Answer

I have wrote a bunch of tests for my implementation, to run them:

```shell
$ cd c-impl
$ make build
$ ./test
All utility tests passed!
test_my_realpath passed!
```

Note that feature `dereferencing symlink` is disabled. To enable it, uncomment the
following code in `my_realpath()`:

```c
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
```

I disable it for the reason that this is not easy to test, one have to create link first:

```shell
$ ln -s ../README.md link1
$ ln -s ./main.c link2
$ ln -s /////home/ link3
$ pwd
/home/steve/Documents/workspace/The-Linux-Programming-Interface/exercise/Ch18/18-3/c-impl
```

Then change `main.c` to this:

```c
#include "test.c"
#include "realpath.h"

int main(void)
{
	//		test_utility();
	//		test_realpath();

	char *res1 = my_realpath("link1", NULL);
	char *res2 = my_realpath("link2", NULL);
	char *res3 = my_realpath("link3", NULL);
	assert(res1 != NULL && res2 != NULL && res3 != NULL);
	printf("res1: %s\nres2: %s\nres3: %s\n", res1, res2, res3);
	free(res1);
	free(res2);
	free(res3);
}
```

```shell
$ make build
$ ./test
res1: /home/steve/Documents/workspace/The-Linux-Programming-Interface/exercise/Ch18/18-3/README.md
res2: /home/steve/Documents/workspace/The-Linux-Programming-Interface/exercise/Ch18/18-3/c-impl/main.c
res3: /home
```
