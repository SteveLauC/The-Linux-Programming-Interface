/* 
 * QUESTION: write a program to see what happens if we try to longjmp() into a 
 * function that has already returned.
 *
 * ANSWER: the behavior is undefined
 * 
 * $ ./a.out
 * return point is set
 * successfully jumped
 * return point is set
 * [1]    136122 segmentation fault (core dumped)  ./a.out
 */

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
