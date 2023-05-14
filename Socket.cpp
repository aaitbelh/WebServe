/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamellal <mamellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:42:58 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/12 10:51:49 by mamellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(std::string host, std::string service)
{
    creatSocket(host, service);
}

Socket::~Socket()
{

}

void    Socket::creatSocket(std::string& host, std::string& service)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo(host.c_str(), service.c_str(), &hints, &bind_address);
    socketfd = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (socketfd < 0)
    {
        std::cerr<< strerror(socketfd)<<std::endl;
        return ;
    }
    int yes = 1;
    setsockopt(socketfd,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    int b = bind(socketfd, bind_address->ai_addr, bind_address->ai_addrlen);
    if (b)
    {
        std::cerr<<"error in bind(): "<< strerror(b) << std::endl;
        socketfd = -1;
        return ;
    }
    freeaddrinfo(bind_address);
    b = listen(socketfd, SOMAXCONN);
    if (b < 0)
    {
        std::cerr<<"error in listn(): "<< strerror(b)<<std::endl;
        socketfd = -1;
        return ;
    }
}

SOCKET  Socket::operator()()
{
    return (socketfd);
}


int  waitingForClients(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList)
{
    SOCKET max_socket = socketListen;
    for (std::list<Client>::iterator  i = clientList.begin(); i != clientList.end(); i++)
    {
        FD_SET((*i).getSocket(), readSet);
        FD_SET((*i).getSocket(), writeSet);
        if (max_socket < (*i).getSocket())
            max_socket = (*i).getSocket();
    }
    if (select(max_socket + 1, readSet, writeSet, 0, 0) < 0)
    {
        std::cerr<<"waitingForClients listn"<<strerror(errno)<<std::endl;
        return (-1);
    }
    return (0);
}
void    setSocketForReadAndWrite(fd_set *readSet, fd_set *writeSet, SOCKET socketListen)
{
    FD_ZERO(readSet);
    FD_SET(socketListen, readSet);
    FD_ZERO(writeSet);
    FD_SET(socketListen, writeSet);
}

int		acceptNewConnictions(fd_set *readSet, fd_set *writeSet, SOCKET socketListen, std::list<Client>& clientList)
{
    if (FD_ISSET(socketListen, readSet))
    {
        Client  client;
        SOCKET sock = accept(socketListen, (struct sockaddr *)&(client.getAddress()), &(const_cast<socklen_t&>(client.getAddrtLen())));
        client.setSocket(sock);
        if (client.getSocket() < 0)
        {
            std::cerr<< "socket < 0" << strerror(errno) << std::endl;
            return (-1);
        }
        clientList.push_front(client);
    }
    return (0);

}

char* get_name(Client& client)
{
    static char address_buffer[100];
    getnameinfo((struct sockaddr *)&client.getAddress(),
                client.getAddrtLen(),
                address_buffer, sizeof(address_buffer), 0, 0,
                NI_NUMERICHOST);
    return address_buffer;
}

int		acceptREADsocket(fd_set *readSet, fd_set *writeSet, Client& client, std::list<Client>& clientList, std::list<Client>::iterator& i)
{
    try
    {
    if (FD_ISSET(client.getSocket(), readSet))
    {
        char    buffer[BUFFER_SIZE + 1];
        int r = recv(client.getSocket(), buffer, BUFFER_SIZE, 0);
        if (r < -1)
        {
            std::cerr<<"error in resv(): "<<strerror(errno)<<std::endl;
            close(client.getSocket());
            clientList.erase(i);
            return (-1);
        }
        buffer[r] = '\0';
        if(r <= 0)
        {
            close(client.getSocket());
            clientList.erase(i);
            return 1;
        }
        if (r > 0)
        {
            Request& request = client.getRequest();
            if (client.getRes().getHeader().empty())
            {
                request.addToReqyest(buffer,r);
                request.parseInfos(i, clientList); // if throwing exiption
                request.setAllinfos(client);
                client.requestvalid = client.getRequest().checkRequest_validation(client);
            }
            else
            {
                if (client.getRequest().getHeaderInfos()["METHOD"] == "POST")
                {
                    try
                    {
                        request.postRequestHandl(buffer, r);
                    }
                    catch (...)
                    {
                        if (request.types_rev[request.getHeaderInfos()["Content-Type"]] == "perl" || request.types_rev[request.getHeaderInfos()["Content-Type"]] == "PHP")
                        {
                            request.exec_cgi(client);
                        }
                        else
                        {
                            try {sendResponse(200, *i);}
                            catch(...){}
                        }
                        request.getMyfile().close();
                        close(i->getSocket());
                        clientList.erase(i);
                        return (0);
                        //     ! send response drop clinet when uplowd is finished 
                    }
                }    
                else
                    request.addToReqyest(buffer, r);
            }
        }
        client.writable = 1;
    }
    if (client.getHeaderInfos()["METHOD"] != "POST" && FD_ISSET(client.getSocket(), writeSet) && client.writable)
    {
        if(client.getHeaderInfos()["METHOD"] == "GET" && !client.requestvalid)
        {
            handlGetRequest(client);
        }
        else if(client.getHeaderInfos()["METHOD"] == "DELETE" && !client.requestvalid)
            handlDeleteRequest(client);
	    sendHeader(client);
		sendBody(client);
    }
    }
    catch (std::exception)
    {
        close(client.getSocket());
        clientList.erase(i);
    }
    return 1;
}
void 			sendResponse(int status, Client& client)
{
    std::map<int, std::string> status_code;
    status_code[__NOTFOUND] = " 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 48\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
    status_code[__BADREQUEST] = " 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 50\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>400 Bad Request</h1></body></html>";
    status_code[__FORBIDDEN] = " 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: 48\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>403 Forbidden</h1></body></html>\r\n";
    status_code[__METHODNOTALLOWED] = " 405 Method Not Allowed\r\nContent-Type: text/html\r\nContent-Length: 57\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>405 Method Not Allowed</h1></body></html>\r\n";
    status_code[__REQUESTTIMEOUT] = " 408 Request Timeout\r\nContent-Type: text/html\r\nContent-Length: 54\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>408 Request Timeout</h1></body></html>";
    status_code[__BADGATEWAY] = " 502 Bad Gateway\r\nContent-Type: text/html\r\nContent-Length: 50\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>502 Bad Gateway</h1></body></html>";
    status_code[__SUCCESS] = " 200 OK\r\nContent-Type: text/html\r\nContent-Length: 40\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>200 OK</h1></body></html>";
    status_code[__NOTIMPLEMENTED] = " 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: 54\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
    status_code[__CONFLICT] = " 409 Conflict\r\nContent-Type: text/html\r\nContent-Length: 47\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>409 Conflict</h1></body></html>";
    status_code[__NOCONTENT] = " 204 No Content\r\nContent-Type: text/html\r\nContent-Length: 49\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>204 No Content</h1></body></html>";
    status_code[__REQUESTTOOLARGE] = " 413 Request Entity Too Large\r\nContent-Type: text/html\r\nContent-Length: 60\r\nConnection: close\r\nServer: Webserv/1.0\r\n\r\n<html><body><h1>413 Request Entity Too Large</h1></body></html>";
    std::string req = status_code[status];
    req.insert(0, client.getHeaderInfos()["VERSION"]);
    int r = send(client.getSocket(), req.c_str(),  req.length(), 0);
    throw std::exception();
}