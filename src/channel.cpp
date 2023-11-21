#include <iostream>
#include <vector>
#include <functional>

#include <sys/socket.h>

#include "include/client.hpp"
#include "include/channel.hpp"

Channel::Channel(std::vector<std::reference_wrapper<Client>> clients_)
{
    clients = clients_;
}

void Channel::add_client(Client &client)
{
    clients.push_back(client);

    for (Client& c : clients)
    {
        std::cout << c.get_username() << '\n';
    }
}

void Channel::write_msg(const std::string &msg)
{
    const char *cmsg = msg.c_str();
        
    for (Client& client : clients)
    {
        client.write_msg(cmsg, msg.length());
    }
}
