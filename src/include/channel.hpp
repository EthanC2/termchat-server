#pragma once

#include <sys/socket.h>
#include <vector>

#include "client.hpp"

class Channel
{
    public:
        Channel(std::vector<Client&> clients = {});
        void add_client(Client &client);
        void write(const std::string &msg);

    private:
        std::vector<Client&> clients;
};
