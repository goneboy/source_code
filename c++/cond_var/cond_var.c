#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void thread_join(void);
void thread_exit(void);
void* thread_child(void* data_in);

int main(int argc, char *argv[])
{
	printf("[parent]: begin\n");
	pthread_t my_thread;

	//initialize mutex
	pthread_mutex_init(&mutex, NULL);

	//initialize condition variables
	pthread_cond_init(&cond, NULL);

	//create child thread
	pthread_create(&my_thread, NULL, thread_child, NULL);

	//call function pthread_join
	thread_join();

	printf("[parent]: end\n");
	return 0;
}

//child thread
void* thread_child(void* data_in)
{
	printf("[child]: Hello\n");
	thread_exit();

	return NULL;
}

//exit thread
void thread_exit(void)
{
	//lock mutex
	pthread_mutex_lock(&mutex);
	global_var = 1;

	//wake up the parent thread
	pthread_cond_signal(&cond);

	//unlock mutex
	pthread_mutex_unlock(&mutex);
}

//join thread
void thread_join(void)
{
	//lock mutex
	pthread_mutex_lock(&mutex);

	//check the condition, if condition TRUE, sleep and wait child thread
	while(global_var == 0)
	{
		pthread_cond_wait(&cond, &mutex);
	}

	//unlock mutex
	pthread_mutex_unlock(&mutex);
}

