/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlGetRequest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 18:02:37 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/04/09 04:26:07 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

void			sendBody(Client& client, std::ifstream &file)
{
    client.getRes().fillTheBody(client);
    std::string req =  client.getRes().getBody();
    int r = send(client.getSocket(), req.c_str(),  req.length(), 0);
}
void			sendHeader(Client& client, std::ifstream& file)
{
    if(!client.getRes().getHeader().empty())
        return;
    client.getRes().fillTheHeader(client);
    std::string req = client.getRes().getHeader();
    std::cout << req << std::endl;
    int r = send(client.getSocket(), req.c_str(), req.length(), 0);
    if(!r)
        throw std::exception();
    
}
void handlGetRequest(Client& client, std::ifstream& file)
{
        sendHeader(client, client.file);
        sendBody(client, client.file);
}