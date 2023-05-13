/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:47 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/13 13:52:24 by aaitbelh         ###   ########.fr       */
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
    types_rev["application/x-httpd-php"] = "php";
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
int Request::checkRequest_validation(Client& client)
{
    int rvalue = 0;
    std::string allowedchars  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    for(size_t i = 0; i < HeaderInfos["URI"].size(); ++i)
    {
        if(allowedchars.find(HeaderInfos["URI"][i]) == std::string::npos)
        {
            client.getRes().getHeader() = setInfos_header(client, client.server.error_page[400], &rvalue);
            changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 400 Bad Request");
            if(rvalue)
                sendResponse(400, client);
            return 1;
        }
    }
    if(HeaderInfos["METHOD"] == "POST" && !rvalue)
    {
        if(!HeaderInfos.count("Content-Length") && !HeaderInfos.count("Transfer-Encoding")){
            setInfos_header(client, client.server.error_page[400], &rvalue);
            changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 400 Bad Request");
            if(rvalue)
                rvalue = 400;
        }
        else if(HeaderInfos.count("Transfer-Encoding") && HeaderInfos["Transfer-Encoding"] != "chunked"){
            sendResponse(501, client);
        }
        else if(HeaderInfos["URI"].length() > 2048){
            setInfos_header(client, client.server.error_page[414], &rvalue);
            if(rvalue)
                rvalue = 414;
        }
    }
    if(rvalue)
        sendResponse(rvalue, client);
    return 0;
}
std::string GetquerySting(std::string &URI)
{
    if(URI.find("?") != std::string::npos)
    {
        std::string str;
        str = URI.substr(URI.find("?"));
        URI = URI.substr(0, URI.find("?"));
        return str;
    }
    return "";
}
void Request::parseInfos(std::list<Client>::iterator& i, std::list<Client>& clientList)
{
    size_t pos = httpRequest.find(" ");
    HeaderInfos["METHOD"] =  httpRequest.substr(0, pos);
    HeaderInfos["URI"] = httpRequest.substr(pos + 1, httpRequest.find(" ", pos + 1) - pos - 1);
    // HeaderInfos["query"] = GetquerySting(HeaderInfos["URI"]);
    pos = httpRequest.find(" ", pos + 1);
	HeaderInfos["VERSION"] = httpRequest.substr(pos + 1, httpRequest.find("\r\n") - pos - 1);
    pos = httpRequest.find("\r\n") + 2;
    httpRequest = httpRequest.substr(pos);
    pos = 0;
    for(;httpRequest.size();)
    {
        if(httpRequest[0] == '\r' && httpRequest[1] == '\n')
        {
            httpRequest = httpRequest.substr(2);
            break;
        }
        HeaderInfos[httpRequest.substr(pos, httpRequest.find(":"))] = httpRequest.substr(httpRequest.find(": ") + 2, httpRequest.find("\r\n") - (httpRequest.find(": ") + 2));
        httpRequest = httpRequest.substr(httpRequest.find("\r\n") + 2);
    }
    std::map<std::string, std::string>::iterator it;
    // for (it = HeaderInfos.begin(); it != HeaderInfos.end(); ++it) {
    //     std::cout << it->first << ": " << it->second << std::endl;
    // }
    if (HeaderInfos["METHOD"] == "POST")
    {
        totalBytes = atol(HeaderInfos["Content-Length"].c_str());
        openFile(types_rev[HeaderInfos["Content-Type"]]);
    }
}
void    Request::openFile(std::string& extention)
{
    std::stringstream ss;

    srand(time(0));
    ss << rand();
    ss << ".";
    if (!extention.length())
        extention = "no_extention";
    ss << extention;
    std::string name(ss.str());
    MyFilename = name;
    MyFile.open(name, std::ios::app);
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
        throw exception();

    }
    return buffer + i;
}
std::fstream&    Request::getMyfile(){return (MyFile);}

void    Request::postRequestHandl(const char *buffer, int r)
{

    if (HeaderInfos["Transfer-Encoding"] != "chunked")
    {
        MyFile.write(buffer, r);
        resevedBytes += r;
        if (resevedBytes >= totalBytes)
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
                    std::cout<<"   :: "<<chunkedSize<<std::endl,buffer = removeContentLinght(const_cast<char*>(buffer), &r), std::cout<<"   :: "<<chunkedSize<<std::endl;
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


size_t  Request::getLnght(){ return (resevedBytes);}
void    Request::addToReqyest(const char *req, int r)
{
    httpRequest.append(req, r);
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

std::vector<t_location>::iterator getTheLocation(Client& client, t_server &server)
{
    std::string tmp = client.GetClientinfos().URI;
    std::vector<t_location>::iterator tmp_it = server.locations.end();
    while(!tmp.empty())
    {
        for(std::vector<t_location>::iterator it = server.locations.begin(); it != server.locations.end(); ++it){
            if(it->location_path == tmp)
                return it; 
            if(it->location_path == "/")
                tmp_it = it;
        }
        tmp = tmp.erase(tmp.rfind("/"));
    }
    return tmp_it;
}

void	matchTheLocation(Client& client, std::vector<t_server> servers)
{
	bool isfounded = 0;
	struct all_infos rootLocation;
	std::string path;
	for(size_t i = 0; i < servers.size(); ++i)
	{
        std::vector<t_location>::iterator it = getTheLocation(client, servers[i]);
		if(it != servers[i].locations.end())
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
				for(size_t i = 0; i < 3; ++i)
					tmpstruct.allow_methods[i] = 0;
			if(it->isExist("autoindex") && it->getElemetnBylocation("autoindex").front() == "1")
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
	if(!isfounded)
    {
        int rvalue = 0;
        setInfos_header(client, client.server.error_page[404], &rvalue);
        if(rvalue)
            sendResponse(404, client);
    }
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

void Request::exec_cgi(Client &client)
{
    std::cout << "GOT TO THE CGI " << std::endl;
	char **env = (char **)malloc(sizeof(char **) * 5);
	int fd = open("resp", O_RDWR | O_CREAT, 0666);
	char *arg[3];
    env[0] = strdup(("METHOD="+HeaderInfos["METHOD"]).c_str()); 
    env[1] = strdup(("Content-Length="+HeaderInfos["Content-Length"]).c_str()); 
    env[2] = strdup(("Content-Type="+HeaderInfos["Content-Type"]).c_str()); 
    env[3] = strdup(("QUERY_STRING="+HeaderInfos["query"]).c_str()); 
    env[4] = strdup(("HTTP_COOKIE="+HeaderInfos["HTTP_COOKIE"]).c_str()); 
    env[5] = strdup("/Users/aaitbelh/Desktop/mamellaweb/f.php");
	env[6] = NULL;
    std::list<std::string>::iterator it = client.GetClientinfos().cgi_pass.begin();
    ++it;
    arg[0] = strdup("php-cgi");
	arg[1] = strdup("/Users/aaitbelh/Desktop/mamellaweb/f.php");
	arg[2] = NULL;

    int file = open("body", O_RDWR | O_CREAT | O_TRUNC);
    char buffer[1024];
    if(client.getHeaderInfos()["METHOD"] == "POST"){
        while (!MyFile.eof()) {
            MyFile.read(buffer, sizeof(buffer));
            ssize_t bytesRead = MyFile.gcount();
            write(file, buffer, bytesRead);
        }
    }
	pid_t f = fork();
	if(f == 0)
	{
		dup2(fd, 1);
		close(fd);
        if(HeaderInfos["METHOD"] == "POST")
        {
            fd = open(MyFilename.c_str(), O_RDWR | O_CREAT | O_TRUNC);
            dup2(fd, 0);
            close(fd);    
        }
		execve(arg[0], arg, env);
        std::cout << "execve failure" <<std::endl;
        exit(1);
	}
    int status = 0;
	while(waitpid(f, &status, WNOHANG))
    {
        if(WIFEXITED(status))
        {
            break;
        }
    }
    if(HeaderInfos["METHOD"] == "GET")
    {
        close(fd);
        client.file.clear();
        client.file.close();
        client.file.open("resp");
        std::string &header = client.getRes().getHeader();
        struct stat b;
        stat("resp", &b);
        std::stringstream s;
        s << b.st_size;
        header.append("Content-Length: " + s.str() + "\r\n");
    }
    throw std::exception();
}
