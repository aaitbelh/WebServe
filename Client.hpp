/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:26 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/01 21:04:10 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "includes.hpp"
#include "pars/parsing.hpp"

class Socket;
class Request;
class Response;
struct all_infos
{
	std::string host;
	std::string server_name;
	std::string URI;
	std::string upload_loc;
	std::list<std::string> index_files;
	int allow_methods[3];
	bool autoindex;
	std::string root;
	bool return_;
	std::string return__;
	bool cgi_pass_;
	std::list<std::string> cgi_pass;
	struct s_location location_div;
	void setTodefault()
	{
		for(size_t i = 0; i < 3; ++i)
			allow_methods[i] = 0;
		autoindex = 0;
		return_ = 0;
		cgi_pass_ = 0;
	}
	
};
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
	bool isitnew();
	ParsConf	parsingInfos;
	struct all_infos& GetClientinfos();
	std::string dir_body;
	std::string dirname;
private:
    SOCKADDRE   address;
    SOCKET      sockett;
    socklen_t address_length;
    Request     request;
    Response    response;
	struct all_infos allinfos;
	bool is_new;
};


#endif