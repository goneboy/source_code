#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "socket.h"

// for AF_INET
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
	// socket descriptor
	int socketDes; 
	int addressLen;

	// struct sockaddr_in, contains information about AF_INET
	struct sockaddr_in address;
	int retVal;
	char str[30];

	// 1. create socket for client
	socketDes = socket(AF_INET, SOCK_STREAM, 0);

	// 2. set name and assign connection address to socket 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("10.0.2.15");
	address.sin_port = htons(9734);

	addressLen = sizeof(address);

	// 3. connect
	retVal = connect(socketDes, (struct sockaddr *) &address, addressLen);

	// if error occurs
	if(retVal != 0)
	{
		perror("client problem!\n");
		exit(1);
	}

	// 4. after connect successfully, read and write data

	while(TRUE)
	{
		printf("[client]: ");
		fgets(str, sizeof(str), stdin);
		write(socketDes, &str, sizeof(str));

		read(socketDes, &str, sizeof(str));
		printf("[server]: %s", str);

		if(strncmp(str, "exit", 4) == 0)
		{
			printf("exit now\n");

			// break the loop
			break;
		}
	}
	

	// 5. close the socket
	close(socketDes);

	return 0;
}
