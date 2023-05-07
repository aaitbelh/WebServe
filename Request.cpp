/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:47 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/07 18:02:38 by aaitbelh         ###   ########.fr       */
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
    types_rev["text/php"] = "php";
    types_rev["text/x-perl-script"] = "perl";
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

void Request::parseInfos(std::list<Client>::iterator& i, std::list<Client>& clientList)
{
    size_t pos = httpRequest.find(" ");
    HeaderInfos["METHOD"] =  httpRequest.substr(0, pos);
    HeaderInfos["URI"] = httpRequest.substr(pos + 1, httpRequest.find(" ", pos + 1) - pos - 1);
    pos = httpRequest.find(" ", pos + 1);
	HeaderInfos["VERSION"] = httpRequest.substr(pos + 1, httpRequest.find("\r\n") - pos - 1);
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
    //mamella need to manage the error pages to made it easer to me for get them from the config file
    // if(1)
    // {
    //     std::string path = "public/404.html";
    //     setInfos_header(*i, path);
    // }
    if (HeaderInfos["METHOD"] == "POST")
    {
        if (HeaderInfos["Transfer-Encoding"] != "chunked")
            openFile(types_rev[HeaderInfos["Content-Type"]]);
        else
            openFile(types_rev[HeaderInfos["Content-Type"]], true);
        postRequestHandl(const_cast<char *>(httpRequestTmp.c_str()), httpRequestTmp.length(), i, clientList);
    }
}
void    Request::openFile(std::string& extention, bool append)
{
    std::stringstream ss;

    srand(time(0));
    ss << rand();
    ss << ".";
    ss << extention;
    std::string name(ss.str());
    if (append)
        MyFile.open(name, std::ios::app);
    else
        MyFile.open(name);
}

int stringToHexx(std::string& hexString)
{
    int intValue;

    std::stringstream ss;
    ss << std::hex << hexString; // convert to integer
    ss >> intValue;
    return (intValue);
}

char    *Request::removeContentLinght(char *buffer, int *r)
{
    int i = 0;
    while (buffer[i] && buffer[i] != '\r' && buffer[i + 1] != '\n')
        i++;
    if (!buffer[i])
    {
        std::cout<<"11\n";
        chunkOfChuk.append(buffer);
        return NULL;
        std::cout<<"22\n";
    }
    std::string tem(buffer, buffer + i);
    std::cout<<"<<<<: "<<tem<<"\n...  "<<i<<std::endl;
    std::cout<<"rrrr: "<<*r<<std::endl;
    i += 2;
    *r -= i;
    chunkedSize = stringToHexx(tem);//std::strtol(buffer, NULL, 16); ;//
    std::cout<<">>>>: "<<chunkedSize<<std::endl;
    if (!chunkedSize)
    {
        std::cout << "GOT HREE" << std::endl;
        throw exception();

    }
    return buffer + i;
}

void    Request::postRequestHandl(const char *buffer, int r, std::list<Client>::iterator& i, std::list<Client>& clientList)
{
    try
    {
    if (HeaderInfos["Transfer-Encoding"] != "chunked")
    {
        MyFile.write(buffer, r);
        throw std::exception();
    }
    else
    {
            std::string tem(chunkOfChuk + std::string(buffer, r));
            if (chunkOfChuk.size())
            {
                r = tem.length();
                buffer = removeContentLinght(const_cast<char *>(tem.c_str()), &r);
                if (!buffer)
                    return ;
            }
            if (chunkedSize < r && chunkedSize > 0)
            {
                MyFile.write(buffer, chunkedSize);
                buffer = buffer + chunkedSize + 2;
                r -= chunkedSize + 2;
                chunkedSize = 0;
                chunkOfChuk.clear();
				resevedBytes += chunkedSize;
            }
            if (r > 0)
            {
                if (chunkedSize <= 0)
                    std::cout<<"   :: "<<chunkedSize<<std::endl,buffer = removeContentLinght(const_cast<char*>(buffer), &r);
                if (!buffer)
                    return ;
                MyFile.write(buffer, r);
                chunkedSize -= r;
                std::cout<<"..   >"<<chunkedSize<<std::endl;
                chunkOfChuk.clear();
				resevedBytes += r;
            }
        
        //catch(const int& i) {printf("ZXCVBNM<>LKJHGFDXCVBNM\n");exit(0);    }
    }   
    }
    catch (const int d)
    {
            MyFile.close();
            sendResponse(200, *i);
            close(i->getSocket());
            clientList.erase(i);
        //     ! send response drop clinet when uplowd is finished 
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

void setTherootLocation(Client &client, std::vector<t_server>& servers)
{
	for(size_t i = 0; i < servers.size(); ++i)
	{
		for(std::vector<t_location>::iterator it = servers[i].locations.begin();it != servers[i].locations.end(); ++it)
		{
			if(it->location_path == "/")
			{
				client.GetClientinfos().location_div = *it;
				return ;
			}
		}
	}
}

void	matchTheLocation(Client& client, std::vector<t_server> servers)
{
	bool isfounded = 0;
	struct all_infos rootLocation;
	std::string path;
	for(size_t i = 0; i < servers.size(); ++i)
	{
		for(std::vector<t_location>::iterator it = servers[i].locations.begin();it != servers[i].locations.end(); ++it)
		{
			if(client.GetClientinfos().URI.substr(0, it->location_path.size()) == it->location_path && it->location_path.size() > path.size())
			{
				isfounded = 1;
				struct all_infos &tmpstruct = client.GetClientinfos();
				tmpstruct.server_name = servers[i].server_map["server_name"].front();
				if(it->isExist("allow_methods"))
				{
					std::list<std::string> lst = it->getElemetnBylocation("allow_methods");
					if(lst.size())
					{
						for(std::list<std::string>::iterator lst_it = lst.begin(); lst_it != lst.end(); ++lst_it)
						{
							if(*lst_it == "GET")
								tmpstruct.allow_methods[0] = 1;
							else if(*lst_it == "POST")
								tmpstruct.allow_methods[1] = 1;
							else if(*lst_it == "DELETE")
								tmpstruct.allow_methods[2] = 1;				
						}
					}
					lst.clear();
				}
				else
				{
					for(size_t i = 0; i < 3; ++i)
						tmpstruct.allow_methods[i] = 0;
				}
				if(it->isExist("autoindex"))
					tmpstruct.autoindex = 1;	
				else
					tmpstruct.autoindex = 0;
				if(it->isExist("return"))
				{
					tmpstruct.return_ = 1;
					tmpstruct.return__ = it->getElemetnBylocation("return").front();
				}
				else
					tmpstruct.return_ = 0;
				if(it->isExist("cgi_pass"))
				{
					tmpstruct.cgi_pass_ = 1;
					tmpstruct.cgi_pass = it->getElemetnBylocation("cgi_pass");
				}
				else
					tmpstruct.cgi_pass_ = 0;
				tmpstruct.index_files = it->getElemetnBylocation("index");
				if(it->isExist("root"))
					tmpstruct.root = it->getElemetnBylocation("root").front();
				tmpstruct.location_div = *it;
				return ;
			}
		}	
	}
	if(!isfounded)
		client.GetClientinfos() = rootLocation;
}

std::vector<t_server> GettheServer(ParsConf &pars, Client &client)
{
	std::vector<t_server> servers;
	std::string::iterator it =  client.GetClientinfos().host.begin() + client.GetClientinfos().host.find(":");
	client.GetClientinfos().host.erase(it, client.GetClientinfos().host.end());
	for(size_t i = 0; i < pars.servers.size(); ++i)
	{
		if(client.GetClientinfos().host == pars.servers[i].getFromServerMap("host").front())
        {
			servers.push_back(pars.servers[i]);
        }
	}
	return servers;
}
void        Request::setAllinfos(Client &client)
{
	struct all_infos &tmpstruct = client.GetClientinfos();
	struct ParsConf &tmp_parsstruct = client.parsingInfos;
	tmpstruct.host = this->getHeaderInfos()["Host"];
	tmpstruct.URI = this->getHeaderInfos()["URI"];
	std::vector<t_server> servers = GettheServer(client.parsingInfos, client);	
	matchTheLocation(client, servers);
}