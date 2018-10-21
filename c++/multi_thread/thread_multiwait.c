#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef MAX_NUM
#define MAX_NUM 5
#endif

#ifndef THREAD_NUM
#define THREAD_NUM 5
#endif

void* do_thread(void* data_in);

int main(int argc, char *argv[])
{
	pthread_t my_thread[THREAD_NUM];
	void *thread_ret;
	int ret;

	//create the child threads
	for(int i = 1; i < THREAD_NUM; i++)
	{
		ret = pthread_create(&my_thread[i], NULL, do_thread, (void *) &i);

		if(ret)
		{
			perror("[main thread]: create thread error\n");
			exit(EXIT_FAILURE);
		}

		sleep(1);
	}
	
	//wait for the threads
	for(int i = THREAD_NUM - 1; i > 0; i--)
	{
		pthread_join(my_thread[i], &thread_ret);
		//int *thread_return_int = (int *) thread_ret;
		printf("[main thread]: thread %d returns %s\n", i, (char *) thread_ret);
	}

	return 0;
}

void* do_thread(void* data_in)
{
	int *data = (int *) data_in;

	for(int i = 1; i <= MAX_NUM; i++)
	{
		printf("[thread %d]: %d\n", *data, i);
	}

	pthread_exit("Bao Trung");
}