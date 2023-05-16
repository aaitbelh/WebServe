/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamellal <mamellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:41:50 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/16 13:32:31 by mamellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
void Server::serverRun(t_server &server)
{
    signal(SIGPIPE, SIG_IGN);
    // while (true)
    // {
        std::cout << "wtf is going on"<<std::endl;
        fd_set  readSet;
        fd_set  writeSet;
        setSocketForReadAndWrite(&readSet, &writeSet, socketListen());
        if (waitingForClients(&readSet, &writeSet, socketListen(), clientList) < 0)
            throw std::exception();
        acceptNewConnictions(&readSet, &writeSet, socketListen(), clientList);
        clientList.begin()->parsingInfos = this->pars;
        std::list<Client>::iterator i = clientList.begin(), j;
        i->server = server;
        j = i;
        while (i != clientList.end())
        {
            ++j;
            acceptREADsocket(&readSet,&writeSet, *i, clientList, i);
            i = j;
        }
     
    std::cout<<"server 377   \n";
    // }
}
Server::Server(std::string host, std::string port):socketListen(host.c_str(), port.c_str()), clientList()
{
    if (socketListen() < 0)
        throw std::exception();
}

Server::~Server()
{
}

int main(int ac, char **av)
{
    if(ac == 2)
    {
        ParsConf pars;
        pars.countserver(av[1]);
        pars.fill_server();
        size_t index = 0;
        while(1)
        {
            Server s(pars.servers[index % pars.servers.size()].server_map["host"].front(), pars.servers[index % pars.servers.size()].server_map["listen"].front());
		    s.pars = pars;
            s.serverRun(pars.servers[index % pars.servers.size()]);
            std::cout << pars.servers[index % pars.servers.size()].server_map["listen"].front()<<std::endl;
            index++;
        }
        return (0);
    }
    std::cerr<<"... allah ihdiik ashrif _"<<std::endl;
}