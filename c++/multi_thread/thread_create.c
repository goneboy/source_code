#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void* do_loop(void* data_in);

int main(int argc, char *argv[])
{
	pthread_t my_thread;
	int ret;
	// int a = 1;
	// int b = 10;
	// char *ptr_a = "[child thread]: ";
	// char *ptr_b = "[parent thread]: ";
	char *ptr_a;
	char *ptr_b;
	ptr_a = (char *) malloc(32 * sizeof(char));
	strcpy(ptr_a, "child thread]: ");
	ptr_b = (char *) malloc(32 * sizeof(char));
	strcpy(ptr_b, "parent thread]: ");

	//create new thread
	ret = pthread_create(&my_thread, NULL, do_loop, (void *) ptr_a);

	//if return value from pthread_create is not equal 0
	if(ret)
	{
		perror("Create thread error\n");
		exit(EXIT_FAILURE);
	}

	//run do_loop in main thread
	do_loop((void *) ptr_b);

	free(ptr_a);
	free(ptr_b);

	return 0;
}

void* do_loop(void* data_in)
{
	char *data = (char *) data_in;
	// char *data;
	// data = (char *) malloc(32 * sizeof(char));

	// memcpy(data, data_in, strlen(data_in) + 1);

	for(int i = 0; i < 5; i++)
	{
		printf("%s got %d\n", data, i);
	}

	//free(data);

	pthread_exit(NULL);
}