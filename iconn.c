// Title:			iconn.c
// Creator:			John Coleman
// Creation Date:		09/04/2023
// Modification Date:        	08/23/2024
// Desscription:             
// A networking tool for testing various layers of the network stack in Linux.
// At the moment the tool only acts like a basic `socat` command would to where
// it receives traffic from a peer, and responds with that peers source IP and
// source ephemeral port.  This is very useful with container network implementation
// troubleshooting.
//
// Progress:			Work in progress.

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>


#define RECEIVE_BUFFER 512

int main(int argc, char *argv[])
{
	
	// socket creation
	int listener;
	listener = socket(AF_INET,SOCK_STREAM,0);

	// construct listener address and peer address
	struct sockaddr_in host_address, peer_address;
	host_address.sin_family 			= AF_INET;
	host_address.sin_port   			= htons(50080);
	host_address.sin_addr.s_addr			= INADDR_ANY;

	char response[RECEIVE_BUFFER];
	int i = 0;
	while(i<RECEIVE_BUFFER)
	{
		response[i] = '\0';
		i++;
	}
	char ip[sizeof(peer_address)];

	// bind net tuple to socket
	bind(listener, (struct sockaddr*)&host_address, sizeof(host_address));

	// listener socket
	listen(listener,100);

	// create socket for peer
	int peer_socket;
	int peer_socket_size = sizeof(struct sockaddr_in);
	
	while(1)
	{
		peer_socket = accept(listener, (struct sockaddr*)&peer_address, &peer_socket_size);
		if(peer_socket>1)
		{
			fprintf(stdout, "Created socket for peer.\n\n");
		}
		else
		{
			fprintf(stdout, "error creating socket for peer.  Returned: %d", peer_socket);
			return 99;
		}

		inet_ntop(AF_INET, &(peer_address.sin_addr.s_addr), ip, sizeof(peer_address));
		fprintf(stdout, "peer returned value: %x\naddress size: %d\n\n", ntohl(peer_address.sin_addr.s_addr), sizeof(peer_address));
		sprintf(response, "\n\nRequestor IP: %s\nRequestor Port: %d\n\n", ip, ntohs(peer_address.sin_port));
		//response[sizeof(peer_address)+1] = '\n';

		// reply
		send(peer_socket, response, sizeof(response), 0);

		// close out serving socket
		close(peer_socket);
	}

	return 0;
}
