#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;


#ifndef MSG_SIZE
#define MSG_SIZE 128
#endif

// declare the message structure
typedef struct
{
	long mType;
	char mText[MSG_SIZE];
} MESSAGE_BUFFER;


int main(int argc, char *argv[])
{
	



	exit(EXIT_SUCCESS);
}