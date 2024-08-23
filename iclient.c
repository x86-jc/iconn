// Title:			iclient.c
// Creator:			John Coleman
// Creation Date:		11/30/2023
// Modification Date:        	08/23/2024
// Description:             
// A client tool that works alongside iconn.c.
//
// Progress:			Work in progress.

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

int main(int argc, char *argv[])
{
	// check to make sure IP was entered
	if(argv[1]==NULL)
	{
		fprintf(stderr, "This tool needs an IP to be specified.\n\n");
		exit(-1);
	}

	int host_socket;
	host_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(50080);
	socket_address.sin_addr.s_addr = inet_addr(argv[1]);

	int connection = connect(host_socket, (struct sockaddr *)&socket_address, sizeof(socket_address));
	if(connection == -1)
	{
		fprintf(stderr, "Error making connection to remote socket.\n\n");
	}

	char response[1024];
	recv(host_socket, &response, sizeof(response), 0);

	fprintf(stdout, "Response: %s\n", response);

	close(host_socket);

	return 0;
}
