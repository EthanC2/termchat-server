#pragma once

#include <sys/socket.h>
#include <vector>
#include <functional>

#include "client.hpp"

class Channel
{
    public:
        Channel(std::vector<std::reference_wrapper<Client>> clients_ = {});
        void add_client(Client &client);
        void write_msg(const char *msg, size_t len) const;

    private:
        std::vector<std::reference_wrapper<Client>> clients;
};
