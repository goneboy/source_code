#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


using namespace std;

#define MAX_THREAD 5

void* DoThread(void* dataIn);

int main(int argc, char *argv[])
{
	int retVal;

	pthread_t myThread[MAX_THREAD];

	for(int i = 1; i <= MAX_THREAD; i++)
	{
		retVal = pthread_create(&myThread[i], NULL, DoThread, (void *) &i);

		if(retVal)
		{
			cout << "create thread " << i << " error\n";

			exit(EXIT_FAILURE); 
		}
	}

	// wait for child thread
	for(int i = MAX_THREAD; i > 0; i--)
	{
		retVal = pthread_join(myThread[i], NULL);

		if(retVal)
		{
			cout << "wait for child thread " << i << " error\n";

			exit(EXIT_FAILURE); 
		}
	}

	cout << "[main thread]: exit here\n";

	return 0;
}

void* DoThread(void* dataIn)
{
	unsigned int *myData = (unsigned int *) dataIn;

	cout << "[thread " << *myData << "]: hello\n";

	pthread_exit(NULL);
}

