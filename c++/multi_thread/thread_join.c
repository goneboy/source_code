#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
	int a;
	int b;
} MY_ARG;

typedef struct
{
	int x;
	int y;
} MY_RET;

void* my_thread(void* data_in);

int main(int argc, char *argv[])
{
	pthread_t thread_id;
	MY_ARG arg = {10, 90};
	MY_RET *ret;

	//create child thread
	pthread_create(&thread_id, NULL, my_thread, (void*) &arg);

	//wait for child thread terminated
	pthread_join(thread_id, (void **) &ret);

	printf("ret->x = %d, ret->y = %d\n", ret->x, ret->y);

	free(ret);

	return 0;
}

void* my_thread(void* data_in)
{
	MY_ARG *arg;
	arg = (MY_ARG *) data_in;
	printf("arg->a = %d, arg->b = %d\n", arg->a, arg->b);

	MY_RET *ret;
	ret = (MY_RET *) malloc(sizeof(MY_RET));
	ret->x = 1;
	ret->y = 9;

	//return ret;
	pthread_exit(ret);
}

