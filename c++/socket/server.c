#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"

// header for AF_INET
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int serverDes;
	int clientDes;
	int serverLen;
	int clientLen = 0;

	struct sockaddr_in serverAdd;
	struct sockaddr_in clientAdd;

	// store return value
	// int retVal;
	char str[30];

	// 1. create socket for server
	serverDes = socket(AF_INET, SOCK_STREAM, 0);

	// if error occur
	// if()
	// {

	// }

	// 2. set name and assign connection address to socket
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_addr.s_addr = inet_addr("10.0.2.15");
	serverAdd.sin_port = htons(9734);

	// get the server address size
	serverLen = sizeof(serverAdd);

	// 3. bind the name of socket
	bind(serverDes, (struct sockaddr *) &serverAdd, (socklen_t) serverLen);

	// 4. listen the connections from client, max 5 connections
	listen(serverDes, 5);

	// 5. forever loop to wait and process the connection from client
	printf("server is waiting ...\n");

	// wait and accept the connection
	clientDes = accept(serverDes, (struct sockaddr *) &clientAdd, (socklen_t *) &clientLen);

	// if error occur, -1 is returned
	if(clientDes == -1)
	{
		perror("acception problem!\n");
		exit(1);
	}
	else
	{
		printf("server is ready to server\n");
	}

	while(TRUE)
	{
		// read the data from client
		read(clientDes, &str, sizeof(str));
		printf("[client]: %s", str);

		// send the data to client
		printf("[server]: ");
		fgets(str, sizeof(str), stdin);

		if(strncmp(str, "exit", 4) == 0)
		{
			printf("exit now\n");
			write(clientDes, &str, sizeof(str));

			// break the loop
			break;
		}

		write(clientDes, &str, sizeof(str));
	}

	// 6. close the socket
	close(clientDes);

	return 0;
}