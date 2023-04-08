/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/04/07 23:20:15 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "includes.hpp"


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
    void    openFile(std::string& extention);
private:
    REQUES_TYPE type;
    size_t      resevedBytes;
    int         fd;
    std::string httpRequest;
    std::map<std::string, std::string> HeaderInfos;
    static int        status;
    FILE             *tmp;
    std::ofstream MyFile;
    //...
};





#endif