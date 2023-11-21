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
    std::cout << "Current client count: " << clients.size() << '\n';
    clients.push_back(client);
    std::cout << "Added " << client.get_username() << " to client list. Total: " << clients.size() << " clients\n";

    int i=0;
    for (const Client &client : clients)
    {
        std::cout << i+1 << ". " << client.get_username() << '\n';
        ++i;
    }
}

void Channel::write_msg(const char *msg, size_t len) const
{
    for (const Client &client : clients)
    {
        client.write_msg(msg, len);
    }
}
