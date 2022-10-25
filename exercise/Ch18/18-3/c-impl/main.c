#include "test.c"
#include "realpath.h"

int main(void)
{
	test_utility();
	test_realpath();

	//	char *res1 = my_realpath("link1", NULL);
//	char *res2 = my_realpath("link2", NULL);
//	char *res3 = my_realpath("link3", NULL);
//	assert(res1 != NULL && res2 != NULL && res3 != NULL);
//	printf("res1: %s\nres2: %s\nres3: %s\n", res1, res2, res3);
//	free(res1);
//	free(res2);
//	free(res3);
}
