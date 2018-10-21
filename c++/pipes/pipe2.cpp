#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

using namespace std;


int main(int argc, char *argv[])
{

	int dataProcess;
	int fileDes[2];
	const char data[] = "tran bao trung";
	char buffer[BUFSIZ + 1];
	pid_t forkRetVal;

	memset(buffer, 0, sizeof(buffer));

	// int pipe(int file_descriptor[2]);
	// pipeis passed (a pointer to) an array of two integer file descriptors. 
	// It fills the array with two new file descriptors and returns a zero. 
	// On failure, it returns -1 and sets errno to indicate the reason for failure
	int retVal = pipe(fileDes);

	// create pipe success
	if(retVal == 0)
	{
		// duplicate the parent process with fork()
		forkRetVal = fork();

		switch(forkRetVal)
		{
			// error
			case -1:
				cout << "create new process error\n";
				exit(EXIT_FAILURE);
				break;

			// child process
			case 0:
				// read the data from fileDes[0]
				sleep(1);
				dataProcess = read(fileDes[0], buffer, BUFSIZ);
				cout << "[child]: read " << dataProcess << " bytes: " << buffer << endl;
				exit(EXIT_SUCCESS);	
				break;

			// parent process
			default:
				// write the data to fileDes[1]
				dataProcess = write(fileDes[1], data, strlen(data));
				cout << "[parent] write " << dataProcess << " bytes\n";

				exit(EXIT_SUCCESS);	
				break;
		}

	}
}