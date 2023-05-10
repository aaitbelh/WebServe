/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlGetRequest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 18:02:37 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/05/10 20:03:49 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

void			sendBody(Client& client, std::ifstream &file)
{
	client.getRes().fillTheBody(client);
	std::string req =  client.getRes().getBody();
	int r = send(client.getSocket(), req.c_str(),  req.length(), 0);
	if(!r)
		throw std::exception();
}
void			sendHeader(Client& client, std::ifstream& file)
{
	if(!client.isitnew())
		return;
	std::string req = client.getRes().getHeader();
	// std::cout << "THIS HEADER IS SENDED " << std::endl;
	// std::cout << req << std::endl;
	int r = send(client.getSocket(), req.c_str(), req.length(), 0);
	client.isitnew() = 0;
	
}
void	handlGetRequest(Client& client, std::ifstream& file)
{
	if(!client.getRes().getHeader().empty())
		return;
	client.getRes().fillTheHeader(client);
}