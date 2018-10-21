#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// structure for message queue
struct mBuffer
{
	long mType;
	char mText[100];
} sMessage;

int main(int argc, char *argv[])
{
	key_t key;
	int mID;

	// ftok to generate unique key
	key = ftok("progfile", 65);

	// msgget creates a message queue and returns an identifier
	mID = msgget(key, 0666 | IPC_CREAT);

	// msgrcv to receive the message
	msgrcv(mID, &sMessage, sizeof(sMessage), 1, 0);

	// display the message
	cout << "[ReaderProcess]: Data receive is " << sMessage.mText;

	// destroy the message queue
	msgctl(mID, IPC_RMID, NULL);

	exit(EXIT_SUCCESS);
}