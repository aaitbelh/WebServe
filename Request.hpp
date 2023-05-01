/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/04/30 11:04:55 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "includes.hpp"

class Client;
class Request
{
public:
    Request(/* args */);
    ~Request();
    size_t                  getLnght();
    void                    addToReqyest(const char *req, int r);
    void                    setType(REQUES_TYPE reqType);
    REQUES_TYPE             getType();
    const std::string&     getHttpRequest();
    void parseInfos();
    std::map<std::string, std::string>  types_rev;
    std::map<std::string, std::string>& getHeaderInfos();
    void setToFile(const std::string& str);
    size_t  getResevedByts();
    std::string getContentType();
    void    postRequestHandl(const char *bufefr, int r);
	void    setAllinfos(Client &client);
    char    *removeContentLinght(char *buffer, int *r);
    void    openFile(std::string& extention, bool append = false);
private:
    REQUES_TYPE type;
    size_t      resevedBytes;
    int         fd;
    std::string httpRequest;
    std::map<std::string, std::string> HeaderInfos;
    static int        status;
    std::string         chunkOfChuk;
    size_t              chunkedSize;
    FILE             *tmp;
    std::ofstream MyFile;
};





#endif