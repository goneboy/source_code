#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

// signal handling function
void CatchSignal(int signalNum)
{
	signal(SIGINT, CatchSignal);

	cout << "do not press ctrl+c\n";

	return;
}


int main(int argc, char *argv[])
{
	int count = 100;

	// trap the signal
	signal(SIGINT, CatchSignal);

	while(count <= 100)
	{
		cout << "count " << count-- << endl;

		sleep(1);
	}

	return 0;
}