/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:32 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/04/12 02:02:58 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "includes.hpp"
#include "pars/parsing.hpp"

#define __BADREQUEST 400
#define __FORBIDDEN 403
#define __NOTFOUND 404
#define __METHODNOTALLOWED 405
#define __REQUESTTIMEOUT   408
#define __BADGATEWAY       502
#define __SUCCESS          200
#define __NOTIMPLEMENTED   501
#define __CONFLICT         409
#define __NOCONTENT        204

class Server
{
private:
    Socket  socketListen;
    std::list<Client>   clientList;
    t_server server;
public:
    Server(std::string host, std::string port);
    void serverRun(t_server &server);
    ~Server();
};

//35


#endif