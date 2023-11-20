#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>

#include <cstdio>
#include <array>
#include <vector>
#include <thread>

#include "include/constants.hpp"
//#include "core/include/constants.h"

void handle_client(int fd)
{
    std::array<char,MSG_MAX_LEN> buffer;
    ssize_t nread;

    printf("Spawned thread to handle client with FD: %d\n", fd);
    
    while ((nread = read(fd, buffer.data(), MSG_MAX_LEN)) > 0)
    {
        write(fd, buffer.data(), nread);    
    }

    close(fd);
}

int main()
{
    std::vector<std::thread> threads;
    sockaddr_in servaddr, cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int listenfd, connfd;

    // 1. Listen
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Bind
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(listenfd, (sockaddr*) &servaddr, sizeof(servaddr));

    // 3. Listen
    listen(listenfd, CONNECTION_BACKLOG);

    // 4. Accepts connections, spawning a thread for each connection
    for(;;)
    {
        connfd = accept(listenfd, (sockaddr*) &cliaddr, &clilen);
        printf("Received connection: FD: %d\n", connfd);

        threads.push_back( std::thread(handle_client, connfd) );
    }

    // 4. Join child threads
    for (auto& t : threads) 
    {
        t.join();
    }

    // N. Close connection
    close(listenfd);

    return 0;
}
