/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlGetRequest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamellal <mamellal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 18:02:37 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/05/11 17:40:02 by mamellal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

void			sendBody(Client& client)
{
	client.getRes().fillTheBody(client);
	std::string req =  client.getRes().getBody();
	std::cout << "BODY---> " << req << std::endl;
	int r = send(client.getSocket(), req.c_str(),  req.length(), 0);
	if(!r)
		throw std::exception();
}
void			sendHeader(Client& client)
{
	if(!client.isitnew())
		return;
	std::string req = client.getRes().getHeader();
	int r = send(client.getSocket(), req.c_str(), req.length(), 0);
	client.isitnew() = 0;
	
}
void	handlGetRequest(Client& client)
{
	if(!client.getRes().getHeader().empty())
		return;
	client.getRes().fillTheHeader(client);
}