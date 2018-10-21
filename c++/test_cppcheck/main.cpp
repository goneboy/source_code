#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	int arr[10];
	arr[10] = 0;

	unsigned char a;
	unsigned short b = 640;

	a = static_cast<unsigned char> (b);

	return 0;

}

