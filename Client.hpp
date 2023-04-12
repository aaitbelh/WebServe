/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:26 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/04/12 02:03:43 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "includes.hpp"
#include "pars/parsing.hpp"

class Response;
class Client
{
    public:
    Client(void);
    explicit Client(SOCKET sockett);
    Client(const Client& client);
    
    ~Client();
    const SOCKET&  getSocket();
    void    setSocket(SOCKET sockett);
    const SOCKADDRE& getAddress();
    const socklen_t&  getAddrtLen();
    Request&     getRequest();
    std::string&    getResponse();
    Response&   getRes();
    std::map<std::string, std::string>& getHeaderInfos();
    int    writable;
    bool   is_dir;
    std::ifstream file;
    t_server server;
private:
    SOCKADDRE   address;
    SOCKET      sockett;
    socklen_t address_length;
    Request     request;
    Response    response;

};


#endif