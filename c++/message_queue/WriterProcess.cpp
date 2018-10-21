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
	sMessage.mType = 1;

	cout << "[WriteProcess]: ";
	fgets(sMessage.mText, 100, stdin);

	// msgsnd to send the message
	msgsnd(mID, &sMessage, sizeof(sMessage), 0);

	// display the message
	cout << "[WriteProcess]: Data send is " << sMessage.mText;

	exit(EXIT_SUCCESS);
}

