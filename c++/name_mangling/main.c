#include "header.h"

void sayHello()
{
	printf("hello world\n");

	return;
}


int main(int argc, char* argv[])
{
	sayHello();

	printf("2 + 6 = %d\n", add2Num(2, 6));

	return 0;
}

