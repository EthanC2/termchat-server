#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>

#include <iostream>
#include <cstdio>
#include <array>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

#include "include/constants.hpp"
#include "include/error.hpp"
#include "include/client.hpp"

void handle_client(int fd, sockaddr_in socket)
{
    Client client(fd, socket, "unknown");
    std::array<char,MSG_MAX_LEN> buffer;
    ssize_t nread;

    inet_ntop(AF_INET, &socket.sin_addr.s_addr, buffer.data(), sizeof(socket));
    printf("New connection: %s:%d with fd=%d\n", buffer.data(), socket.sin_port, fd);
    
      
    while ((nread = client.read_msg(buffer.data(), MSG_MAX_LEN-1)) > 0)
    {
        buffer.data()[nread] = '\0';
        std::cout << "[DEBUG] read: \'" << buffer.data() << "\'\n"; 
    
        client.write_msg(buffer.data(), nread);    
    }
}

int main()
{
    std::vector<std::thread> threads;
    //std::mutex channel_mutex;
    //Channel channel;

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

        threads.push_back( std::thread(handle_client, connfd, cliaddr) );
    }

    // 4. Join child threads
    for (auto& t : threads) 
    {
        t.join();
    }

    // N. Close connection
    errchk( close(listenfd), "close");

    return 0;
}
