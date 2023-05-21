/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:01 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/17 17:35:29 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

Client::Client():sockett(-1), writable(0), is_dir(0), is_new(true), is_cgi(false), cgi_finished(false),  status(200)
{
}

Client::Client(SOCKET socktt): sockett(socktt), writable(0), is_dir(0), is_new(true), is_cgi(false), cgi_finished(false)
{
    address_length = sizeof(address);
    
}
Client::Client(const Client& client): sockett(client.sockett), writable(0), is_dir(0), is_new(true), is_cgi(false), cgi_finished(false)
{
    address_length = client.address_length;
    address = client.address;
}

Client::~Client()
{
}

const SOCKET&  Client::getSocket(){return sockett;}
void    Client::setSocket(SOCKET Sockett)
{
    sockett = Sockett;
}
const SOCKADDRE& Client::getAddress()
{
    return address;
}

const  socklen_t&  Client::getAddrtLen()
{
    return (address_length);
}

Request&    Client::getRequest()
{
    return (request);
}
Response&   Client::getRes()
{
    return (response);
}
std::map<std::string, std::string>& Client::getHeaderInfos()
{
    return this->getRequest().getHeaderInfos();
}
std::string&    Client::getResponse() {return (response.getResponse());}
struct all_infos& Client::GetClientinfos()
{ return this->allinfos; }

bool& Client::isitnew()
{
	return this->is_new;
}

//get status code
int& Client::getStatus()
{
    return this->status;
}