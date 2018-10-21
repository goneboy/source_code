#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

// declare and define the global production
int proVal = 2;

// declare semaphore
sem_t mySem;

// the functions execute consumer threads
void* Consumer(void *);


int main(int argc, char *argv[])
{
	int retVal;
	pthread_t myThread;

	// initialize the semaphore, maximum 2 semaphores
	retVal = sem_init(&mySem, 0, 2);

	if(retVal)
	{
		cout << "initialize semaphore error\n";
		exit(EXIT_FAILURE);
	}


	// create the consumer thread
	retVal = pthread_create(&myThread, NULL, Consumer, (void *) 0);

	if(retVal)
	{
		cout << "create consumer thread error\n";
		exit(EXIT_FAILURE);
	}


	// main thread is producer thread

	cout << "[producer]: i'm running\n";

	for(int i = 0; i < 5; i++)
	{
		// increase the product number
		proVal++;

		cout << "[producer]: product number is " << proVal << endl;

		// unlock semaphore, increase the semaphore key
		sem_post(&mySem);

		// sleep 2 second
		sleep(2);
	}


	cout << "all done!\n";

	// destroy the semaphore
	sem_destroy(&mySem);

	exit(EXIT_SUCCESS);
}

void* Consumer(void *)
{
	cout << "[consumer]: i'm running\n";

	while(TRUE)
	{
		// try to lock the semaphore
		sem_wait(&mySem);

		// decrease the product number
		proVal--;

		cout << "[consumer]: product number is " << proVal << endl;

		// sleep 1 second
		sleep(1);
	}

	// exit the thread
	pthread_exit(NULL);
}