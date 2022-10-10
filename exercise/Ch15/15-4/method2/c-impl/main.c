#include "test.h"

int main(void)
{
	file_that_does_not_exist();
	with_no_permission();
	with_read_permission();
	with_read_write_permission();
	with_read_write_execute_permission();
}
