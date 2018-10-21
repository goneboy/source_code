#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


using namespace std;

// declare global variable
int gVar;

// declare global mutex variable
pthread_mutex_t myMutex;

// declare functions that execute threads
void* IncreaseVar(void *);
void* DecreaseVar(void *);


int main(int argc, char *argv[])
{
	int retVal;
	pthread_t threadInc;
	pthread_t threadDec;

	// create mutex
	retVal = pthread_mutex_init(&myMutex, NULL);

	if(retVal)
	{
		cout << "create mutex error\n";

		exit(EXIT_FAILURE);
	}

	// create thread increase variable
	retVal = pthread_create(&threadInc, NULL, IncreaseVar, (void *) 0);

	if(retVal)
	{
		cout << "create thread error\n";

		exit(EXIT_FAILURE);
	}

	// create thread decrease variable
	retVal = pthread_create(&threadDec, NULL, DecreaseVar, (void *) 0);

	if(retVal)
	{
		cout << "create thread error\n";

		exit(EXIT_FAILURE);
	}

	// main thread
	sleep(20);
	cout << "[main thread]: exit here\n";

	return 0;
}


void* IncreaseVar(void *dataIn)
{
	// lock mutex
	pthread_mutex_lock(&myMutex);

	for(int i = 1; i <= 5; i++)
	{
		cout << "[thread Increase]: " << ++gVar << endl;

		// increase the global variable
		//gVar++;

		sleep(1);
	}

	cout << "[thread Increase]: Finished\n";

	// unlock mutex
	pthread_mutex_unlock(&myMutex);

	// exit the thread
	pthread_exit(NULL);
}

void* DecreaseVar(void *dataIn)
{
	// lock mutex
	pthread_mutex_lock(&myMutex);

	for(int i = 1; i <= 5; i++)
	{
		cout << "[thread Decrease]: " << --gVar << endl;

		// decrease the global variable
		//gVar--;

		sleep(1);
	}

	cout << "[thread Decrease]: Finished\n";

	// unlock mutex
	pthread_mutex_unlock(&myMutex);

	// exit the thread
	pthread_exit(NULL);
}

