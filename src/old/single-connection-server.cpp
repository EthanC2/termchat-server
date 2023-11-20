#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "core/include/constants.h"

#define BACKLOG 3
#define BUFLEN 2000

#define errchk(expr, msg) \
    do                          \
    {                           \
        if ((expr) == -1)       \
        {                       \
            perror(msg);        \
            exit(errno);        \
        }                       \
    } while (0)                 \

int main()
{
	int server_socket_fd, client_socket_fdet_fd, socket_len, nread;
	struct sockaddr_in server, client;
	char buffer[BUFLEN];
	
	// 1. Create socket
	errchk( server_socket_fd = socket(AF_INET , SOCK_STREAM , 0), "socket" );
	
    // 2. Bind socket to IP
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	errchk( bind(server_socket_fd, (struct sockaddr *) &server , sizeof(server)), "bind");
	
	// 3. Listen for connections
	listen(server_socket_fd, BACKLOG);
	
	// 4. Accept incoming connections
	socket_len = sizeof(struct sockaddr_in);
	errchk( client_socket_fdet_fd = accept(server_socket_fd, (struct sockaddr *) &client, (socklen_t*) &socket_len), "accept");
	
	// 5. Echo incoming messages from the client
	while((nread = recv(client_socket_fdet_fd , buffer , BUFLEN, 0)) > 0)
	{
		//Send the message back to client
		write(client_socket_fdet_fd, buffer, nread);
	}
	
	if(nread == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(nread == -1)
	{
		perror("recv failed");
	}
	
	return 0;
}
