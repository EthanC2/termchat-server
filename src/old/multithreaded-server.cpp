#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
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

void *connection_handler(void*);

int main()
{
    pthread_t sniffer_thread;
	int server_socket_fd, client_socket_fd, socket_len, nread;
	struct sockaddr_in server, client;
    int *new_sock;
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
    while( (client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &client, (socklen_t*) &socket_len)) )
	{
		puts("Connection accepted");

		new_sock = (int*) malloc(1);
		*new_sock = client_socket_fd;

		if( pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
    }

	//Now join the thread, so that we dont terminate before the thread
	pthread_join(sniffer_thread, NULL);



	// 5. Echo incoming messages from the client
	while((nread = recv(client_socket_fd , buffer , BUFLEN, 0)) > 0)
	{
		//Send the message back to client
		write(client_socket_fd, buffer, nread);
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


void *connection_handler(void *socket_desc)
{
    int sock = *(int*) socket_desc;
    ssize_t nread;
    char client_message[2000];
    const char *message = "Greetings! I am your connection handler\n";

    write(sock , message , strlen(message));

    //Receive a message from client
	while( (nread = recv(sock, client_message, 2000 , 0)) > 0 )
	{
		//Send the message back to client
		write(sock, client_message, strlen(client_message));
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

	free(socket_desc);

    return 0;
}
