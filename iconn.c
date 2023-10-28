// Title:			iconn.c
// Creator:			John Coleman
// Creation Date:		09/04/2023
// Modification Date:        	10/28/2023
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


#define RBUF 512

int main(int argc, char *argv[])
{
	
	// socket creation
	int lsock;
	lsock = socket(AF_INET,SOCK_STREAM,0);

	// construct listener address and peer address
	struct sockaddr_in hostaddr, peeraddr;
	hostaddr.sin_family 			= AF_INET;
	hostaddr.sin_port   			= htons(50080);
	hostaddr.sin_addr.s_addr		= INADDR_ANY;

	char response[RBUF];
	int i = 0;
	while(i<RBUF)
	{
		response[i] = '\0';
		i++;
	}
	char ip[sizeof(peeraddr)];

	// bind net tuple to socket
	bind(lsock,(struct sockaddr*)&hostaddr,sizeof(hostaddr));

	// listener socket
	listen(lsock,100);

	// create socket for peer
	int peersock;
	int peersock_size = sizeof(struct sockaddr_in);
	
	while(1)
	{
		peersock = accept(lsock,(struct sockaddr*)&peeraddr,&peersock_size);
		if(peersock>1)
		{
			fprintf(stdout, "Created socket for peer.\n\n");
		}
		else
		{
			fprintf(stdout, "error creating socket for peer.  Returned: %d", peersock);
			return 99;
		}

		inet_ntop(AF_INET, &(peeraddr.sin_addr.s_addr), ip, sizeof(peeraddr));
		fprintf(stdout, "peer returned value: %x\naddress size: %d\n\n", ntohl(peeraddr.sin_addr.s_addr), sizeof(peeraddr));
		sprintf(response, "\n\nRequestor IP: %s\nRequestor Port: %d\n\n", ip, ntohs(peeraddr.sin_port));
		//response[sizeof(peeraddr)+1] = '\n';

		// reply
		send(peersock,response,sizeof(response),0);

		// close out serving socket
		close(peersock);
	}

	return 0;
}
