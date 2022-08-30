#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

void set_point(void);

int main(void)
{
	set_point();

	longjmp(env, 1);

	exit(EXIT_SUCCESS);
}

void set_point(void)
{
	switch (setjmp(env)) {
	case 0:
		printf("return point is set\n");
		break;
	case 1:
		printf("successfully jumped\n");
		break;
	}
}
