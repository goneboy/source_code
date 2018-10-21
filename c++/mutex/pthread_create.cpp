#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void* DoThread(void *);

int main(int argc, char *argv[])
{
	int retVal;
	char* threadRetVal;
	pthread_t myThread;

	// call the init thread system call
	retVal = pthread_create(&myThread, NULL, DoThread, (void *) 0);

	if(retVal)
	{
		cout << "create thread error\n";
	}

	// wait for child thread finished
	retVal = pthread_join(myThread, (void **) &threadRetVal);

	if(retVal)
	{
		cout << "[main thread]: wait for child thread error\n";
	}

	cout << threadRetVal;

	//sleep(3);
	cout << "[main thread]: exit here\n";

	return 0;
}

void* DoThread(void *)
{
	for(unsigned int i = 1; i <= 10; i++)
	{
		cout << i << endl;

		sleep(1);
	}

	pthread_exit((void *) "[child thread]: bye bye\n");
}

