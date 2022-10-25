#include "getcwd.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char * res = my_getcwd();
	if (res == NULL) {
		perror(NULL);
	}

	printf("%s\n", res);
	free(res);
}