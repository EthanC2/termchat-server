#include <sys/socket.h>
#include <vector>

#include "client.hpp"

Channel::Channel(std::vector<Client&> clients = {})
{
    clients = clients;
}

void add_client(Client &client)
{
    clients.push_back(client);
}

void write(const std::string &msg)
{
    for (Client& client : clients)
    {
        client.write(msg);
    }
}
