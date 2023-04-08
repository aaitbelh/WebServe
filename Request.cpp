/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:47 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/04/07 23:35:29 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

Request::Request(/* args */): resevedBytes(0)
{
    types_rev["text/html"] = "html";
    types_rev["text/html"] = "htm";
    types_rev["text/plain"] = "txt";
    types_rev["image/jpg"] = "jpg";
    types_rev["image/jpeg"] = "jpeg";
    types_rev["image/png"] = "png";
    types_rev["image/gif"] = "gif";
    types_rev["text/css"] = "css";
    types_rev["application/javascript"] = "js";
    types_rev["application/json"] = "json";
    types_rev["application/pdf"] = "pdf";
    types_rev["audio/mpeg"] = "mp3";
    types_rev["video/mp4"] = "mp4";
    types_rev["image/x-icon"] = "ico";
    types_rev["image/svg+xml"] = "svg";
    types_rev["application/octet-stream"] = "default";
}

Request::~Request()
{
}

std::map<std::string, std::string>& Request::getHeaderInfos()
{
    return HeaderInfos;
}

void Request::setToFile(const std::string&  str)
{
    if(HeaderInfos.count("Content-Length"))
    {
        fwrite(str.c_str(), sizeof(char), str.size(), tmp);
    }
}

void Request::parseInfos()
{

    HeaderInfos["METHOD"] =  httpRequest.substr(0, httpRequest.find(" "));
    HeaderInfos["URI"] = httpRequest.substr(httpRequest.find(" ") + 1, httpRequest.find("HTTP") - (httpRequest.find(" ") + 2));
    size_t pos = 0;
    pos = httpRequest.find("\r\n") + 2;
    std::string httpRequestTmp = httpRequest.substr(pos);
    pos = 0;
    for(;httpRequestTmp.size();)
    {
        if(httpRequestTmp[0] == '\r' && httpRequestTmp[1] == '\n')
        {
            httpRequestTmp = httpRequestTmp.substr(2);   
            break;
        }
        HeaderInfos[httpRequestTmp.substr(pos, httpRequestTmp.find(":"))] = httpRequestTmp.substr(httpRequestTmp.find(": ") + 2, httpRequestTmp.find("\r\n") - (httpRequestTmp.find(": ") + 2));
        httpRequestTmp = httpRequestTmp.substr(httpRequestTmp.find("\r\n") + 2);
    }
    if (HeaderInfos["METHOD"] == "POST")
    {
        openFile(types_rev[HeaderInfos["Content-Length"]]);
        postRequestHandl(httpRequestTmp.c_str(), httpRequestTmp.length());
    }
        //setToFile(httpRequestTmp.substr(2));
}
void    Request::openFile(std::string& extention)
{
    std::stringstream ss;

    srand(time(0));
    ss << rand();
    ss << ".";
    ss << extention;
    std::string name(ss.str());
    std::cout<<"name ---:::  "<<name<<std::endl;
    fd = open(name.c_str(), 664, O_CREAT);
    //     std::cout<<"----------\n";
    //     exit(0);
}

void    Request::postRequestHandl(const char *bufefr, int r)
{
    if (write(fd, bufefr, r) < 0)
    {
        std::cerr<<"can't write"<<std::endl;
    }
}

size_t  Request::getLnght(){ return (resevedBytes);}
void    Request::addToReqyest(const char *req, int r)
{
    httpRequest.append(req, r);
    resevedBytes += r;
}

void    Request::setType(REQUES_TYPE reqType) { type  = reqType;}
REQUES_TYPE    Request::getType() { return(type);}
const std::string&     Request::getHttpRequest() { return (httpRequest);}


size_t  Request::getResevedByts() {return (resevedBytes);}