/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:41:50 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/17 10:37:38 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server& Server::operator=(Server& srv)
{
    return(*this);
}
void Server::serverRun(t_server &server)
{
    fd_set  readSet;
    fd_set  writeSet;
    setSocketForReadAndWrite(&readSet, &writeSet, socketListen());
    if (waitingForClients(&readSet, &writeSet, socketListen(), clientList) < 0)
        throw std::exception();
    acceptNewConnictions(&readSet, &writeSet, socketListen(), clientList);
    if (clientList.size())
    {
        std::list<Client>::iterator i = clientList.begin(), j;
        clientList.begin()->parsingInfos = this->pars;
        i->server = server;
        j = i;
        while (i != clientList.end())
        {
            ++j;
            acceptREADsocket(&readSet,&writeSet, *i, clientList, i);
            i = j;
        }
    }
}
Server::Server(std::string host, std::string port):socketListen(host.c_str(), port.c_str()), clientList()
{
    if (socketListen() < 0)
        throw std::exception();
}
void    Server::operator()(std::string host, std::string port)
{
    socketListen(host.c_str(), port.c_str());
    if (socketListen() < 0)
        throw std::exception();
}
Server::Server()
{
}
Server::~Server()
{
}

int main(int ac, char **av)
{
    try
    {
        if(ac == 2)
        {
            ParsConf pars;
            pars.countserver(av[1]);
            pars.fill_server();
            size_t  sx = 0;
            Server  servers[pars.servers.size()];
            for (size_t i = 0; i < pars.servers.size(); i++)
            {
                servers[i](pars.servers[i].server_map["host"].front(),\
                        pars.servers[i].server_map["listen"].front());
                servers[i].pars = pars;
            }
            
            while(1)
            {
                servers[sx % pars.servers.size()].serverRun(pars.servers[sx % pars.servers.size()]);
                sx++;
                // Server s(pars.servers[sx % pars.servers.size()].server_map["host"].front(),\
                s.pars = pars;
                //          pars.servers[sx % pars.servers.size()].server_map["listen"].front());
                // s.serverRun(pars.servers[sx % pars.servers.size()]);
            }
            return (0);
        }
        std::cerr<<"... allah ihdiik ashrif _"<<std::endl;
    }
    catch(...)
    {
        std::cerr<<"server taaa7 sir 7tal ghda"<<std::endl;
    }
}