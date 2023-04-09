/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:42:56 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/04/09 17:40:54 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "includes.hpp"
class Socket
{
private:
	SOCKET  socketfd;
public:
	Socket(std::string host, std::string service);
	~Socket();
	void    creatSocket(std::string& host, std::string& service);
	SOCKET  operator()();
};


int				waitingForClients(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList);
void			setSocketForReadAndWrite(fd_set *readSet, fd_set *writeSet, SOCKET socketListen);
int				acceptREADsocket(fd_set *readSet, fd_set *writeSet, Client& client, std::list<Client>& clientList, std::list<Client>::iterator& i);
int				acceptWRITEEsocket(fd_set *writeSet, Client& client, std::list<Client>& clientList, std::list<Client>::iterator& i);
int				acceptNewConnictions(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList);
char*			get_name(Client& client);
void			sendHeader(Client& client);
void			sendBody(Client& client);
void 			handlGetRequest(Client& client, std::ifstream& file);
void 			handlDeleteRequest(Client& client);
#endif