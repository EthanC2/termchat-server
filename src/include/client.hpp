#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Client
{
    public:
        Client(int fd_, sockaddr_in socket_);
        Client() = delete;
        Client(Client&) = delete;
        Client& operator=(Client&) = delete;
        ~Client();

        ssize_t read_msg(char *buffer, size_t maxlen) const;
        void write_msg(const char *msg, size_t len) const;
        
        int get_fd() const;
        sockaddr_in get_socket() const;
        const std::string& get_username() const;

        void set_username(std::string name);

    private:
        int fd;
        sockaddr_in socket;
        std::string username;
};
