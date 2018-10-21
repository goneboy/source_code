#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

using namespace std;


int main(int argc, char *argv[])
{
	int dataProcess;
	int retVal;

	// declare 2 file descriptors
	int fileDes[2];

	const char data[] = "bao trung";
		
	char buffer[BUFSIZ + 1];

	memset(buffer, 0, sizeof(buffer));

	// int pipe(int pipefd[2]);
	// pipe() creates a pipe, a unindirectional data channel that can be used for interprocess communication
	retVal = pipe(fileDes);

	if(retVal == 0)
	{
		// write the data to fileDes[1]
		dataProcess = write(fileDes[1], data, strlen(data));
		cout << "wrote " << dataProcess << "bytes\n";

		// read the data from fileDes[0]
		dataProcess = read(fileDes[0], buffer, BUFSIZ);
		cout << "read " << dataProcess << "bytes: " << buffer << endl;

		exit(EXIT_SUCCESS);
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}