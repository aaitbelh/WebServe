/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:54 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/04/30 11:11:06 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "includes.hpp"
class Client;
class Response
{
    public:
        Response(/* args */);
        ~Response();
        std::string&    getResponse();
        std::string find_filename(Client & client);
		void fillTheBody(Client &client);
        void fillTheHeader(Client &client);
        std::map<std::string, std::string> types;
        std::string&    getHeader();
        std::string&    getBody();
        void            setResponse(const std::string & str);
        std::string     getFileType(std::string filename);
		std::string		ind_filename(Client &client);
    private:
        std::string header;
        std::string body;
        std::string httpResponse;
};





#endif