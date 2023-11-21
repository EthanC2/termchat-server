#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>
#include <functional>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>

#include "include/constants.hpp"
#include "include/error.hpp"
#include "include/client.hpp"
#include "include/channel.hpp"

void handle_client(Channel &channel, std::mutex &channel_mutex, int fd, sockaddr_in socket)
{
    Client client(fd, socket);
    char buffer[MSG_MAX_LEN];
    ssize_t nread;

    // 1. Add new client to channel
    {
        std::lock_guard<std::mutex> lock(channel_mutex);
        channel.add_client(client);
    }

    // 2. Echo server
    while ((nread = client.read_msg(buffer, MSG_MAX_LEN-1)) > 0)
    {
        // 2A. Read message from client
        buffer[nread] = '\0';
        std::cout << "[DEBUG: " << client.get_username() << "] read: \'" << buffer << "\'\n"; 
    
        // 2B. Echo message to entire server
        {
            std::lock_guard<std::mutex> lock(channel_mutex);
            channel.write_msg(buffer, nread);
        }
        
        //client.write_msg(buffer, nread);
        //std::cout << "[DEBUG: " << client.get_username() << "] wrote: \'" << buffer << "\'\n"; 
    }
}

int main()
{
    std::vector<std::thread> threads;
    std::mutex channel_mutex;
    Channel channel;

    sockaddr_in servaddr, cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int listenfd, connfd;

    // 1. Listen
    errchk( listenfd = socket(AF_INET, SOCK_STREAM, 0), "socket");

    // 2. Bind
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    errchk( bind(listenfd, (sockaddr*) &servaddr, sizeof(servaddr)), "bind");

    // 3. Listen
    errchk( listen(listenfd, CONNECTION_BACKLOG), "listen");

    // 4. Accepts connections, spawning a thread for each connection
    for(;;)
    {
        errchk( connfd = accept(listenfd, (sockaddr*) &cliaddr, &clilen), "accept");
        printf("Received connection: FD: %d\n", connfd);

        threads.push_back( std::thread(handle_client, std::ref(channel), std::ref(channel_mutex), connfd, cliaddr) );
    }

    // 4. Join child threads
    for (auto& thread : threads) 
    {
        thread.join();
    }

    // N. Close connection
    errchk( close(listenfd), "close");

    return 0;
}
