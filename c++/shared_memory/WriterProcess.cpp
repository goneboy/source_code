#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
{
	// ftok to generate the unique key
	key_t key = ftok("shmfile", 65);

	// shmget returns an indentifier in shmid
	int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

	// shmat to attach to shared memory
	char *str = (char *) shmat(shmid, (void *) 0, 0);

	cout << "[WriteProcess]: ";
	fgets(str, 255, stdin);

	cout << "[WriteProcess]: Data write in memory was " << str;

	// detach from shared memory
	shmdt(str);

	exit(EXIT_SUCCESS);
}


