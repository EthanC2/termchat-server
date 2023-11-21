#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "include/constants.hpp"
#include "include/error.hpp"
#include "include/client.hpp"

Client::Client(int fd_, sockaddr_in socket_)
{
    fd = fd_;
    socket = socket_;
    username = "client-" + std::to_string(fd);
}

Client::~Client()
{
    errchk( close(fd), "close");
}

int Client::get_fd() const
{
    return fd;
}

sockaddr_in Client::get_socket() const
{
    return socket;
}

const std::string& Client::get_username() const
{
    return username;
}

void Client::set_username(std::string name)
{
    username = name;
}

ssize_t Client::read_msg(char *buffer, size_t maxlen) const
{
    ssize_t nread;

    errchk( nread = read(fd, buffer, maxlen), "read");

    return nread;
}

void Client::write_msg(const char *msg, size_t len) const
{
    errchk( write(fd, msg, len), "write");
    std::cout << "[DEBUG: " << get_username() << "] wrote: \'" << msg << "\'\n";
}
