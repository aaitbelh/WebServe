/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:51 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/07 15:51:15 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "Response.hpp"

void Response::fillTheBody(Client &client)
{   
    std::string body;
    char buf[1024];
    client.file.read(buf, 1024);
    body.append(buf, client.file.gcount());
    this->body = body;
    if(client.is_dir && client.file.eof())
    {
		this->body.append(client.dir_body);
		client.dir_body.clear();
		client.is_dir = 0;
    }
    if(client.file.eof() && body.empty())
        throw std::exception(); 
}
std::string&    Response::getHeader()
{
    return (header);
}
std::string&    Response::getBody()
{
    return (body);
}
std::string Response::getFileType(std::string filename)
{
    std::string extention  = filename.substr(filename.find_last_of(".") + 1);
    if(types.count(extention))
        return types[extention];
    return types["default"];
}
int calcluateLen(Client &client)
{ 
	DIR *dir = NULL;
    std::string path = client.dirname;
	std::string nameofdir;
	size_t i; 
	for(i = path.size(); i > 0; --i)
		if(path[i] == '/')
			break;
	nameofdir = path.substr(i + 1, path.size() - i);
    struct dirent *ent = NULL;
    if ((dir = opendir(path.c_str())) != NULL) {
        // path.append("/");
    while ((ent = readdir(dir)) != NULL) {
            if(ent->d_name[0] != '.')
            {
                std::string filename = ent->d_name;
                std::string str = "\n<li><a href=\"" + nameofdir + "/" + filename + "\">" + filename + "</a></li>";
                client.dir_body.append(str);
            }
    }
    client.dir_body.append("\n</ul></body></html>");
    closedir(dir);
    }
    return client.dir_body.length();
}
std::string find_filename(Client &client)
{
	std::string filename;
	std::ifstream file;
	if(client.getHeaderInfos()["URI"] == client.GetClientinfos().location_div.location_path)
	{
		for(std::list<std::string>::iterator it = client.GetClientinfos().index_files.begin(); it !=  client.GetClientinfos().index_files.end(); ++it)
		{
			std::string path =  client.GetClientinfos().root + client.GetClientinfos().location_div.location_path + "/"+ *it;
			file.open(path);
			if(file.is_open() && file.good())
            {
				return path;
            }
		}
	}
	return client.GetClientinfos().root + client.getHeaderInfos()["URI"];
}
void Response::checkRediraction(Client &client)
{
	if(client.GetClientinfos().return_)
	{
		std::string red = client.getHeaderInfos()["VERSION"] + " 301 Moved Permanently\r\n";
		red.append("Location: ");
		red.append(client.GetClientinfos().return__);
		red.append("\r\n");
    	red.append("Connection: close\r\n");
		red.append("Server: " + client.GetClientinfos().server_name + "\r\n");
		red.append("\r\n");
		send(client.getSocket(), red.c_str(), red.length(), 0);
		throw std::exception();
	}
}
std::string setInfos_header(Client &client, std::string filename, int *Rvalue)
{
    std::string header;
    Response tmp;
    std::map<std::string, std::string> &request = client.getRequest().getHeaderInfos();
    struct stat buffer;
    std::stringstream s;
    *Rvalue = 0;
    stat(filename.c_str(), &buffer);
    client.file.open(filename);
    header = request["VERSION"];
    if(client.file.good() && S_ISDIR(buffer.st_mode))
    {
        header += " 200 OK\r\n";   
		client.dirname = filename;
        filename = "listing-dir.html";
        client.is_dir = 1;
        client.file.close();
        client.file.clear();
        client.file.open("listing-dir.html");
    }
    else
    {
        if(client.file.is_open() && !access(filename.c_str(), R_OK))
            header += " 200 OK\r\n";
		else if(client.file.is_open() && client.GetClientinfos().autoindex)
		{
            header += " 404 KO\r\n";
            filename = client.server.error_page[404];
            client.file.close();
            client.file.clear();
            client.file.open(filename);
			if(!client.server.error_page.count(404) || (!client.file.is_open() || !client.file.good())){
                *Rvalue = 404;
                return "";
            }
        }
		else
		{
			sendResponse(403, client);
            header += " 403 KO\r\n";
            filename = client.server.error_page[403];
            client.file.close();
            client.file.clear();
            client.file.open(filename);
			if(!client.server.error_page.count(403) || (!client.file.is_open() || !client.file.good())){
                *Rvalue = 403;
                return "";
            }
		}
    }
    stat(filename.c_str(), &buffer);
    header.append("Connection: close\r\n");
    header.append("Server: ");
	header.append(client.GetClientinfos().server_name);
    std::string date;
    time_t  now = time(0);
    struct tm *t = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", t);
    date = buf;
    header.append("Date: " + date + "\r\n");
    if(client.is_dir)
        s <<  buffer.st_size + calcluateLen(client);    
    else
        s << buffer.st_size;
    header.append("Content-Length: " + s.str() + "\r\n");
    header.append("Content-Type: " + tmp.getFileType(filename) + "\r\n");
    header.append("\r\n");
    return header;
}
void Response::fillTheHeader(Client &client)
{
	checkRediraction(client);
    std::string filename = find_filename(client);
    int Rvalue;
    this->header = setInfos_header(client, filename, &Rvalue);
    if(Rvalue)
        sendResponse(Rvalue, client);
}

void    changeTheHeaderby(Client &client, const std::string &element)
{
    std::string &header = client.getRes().getHeader();
    header.erase(0, header.find("\r\n"));
    header.insert(0,element);
    
}
Response::Response()
{
    types["html"] = "text/html";
    types["htm"] = "text/html";
    types["txt"] = "text/plain";
    types["jpg"] = "image/jpg";
    types["jpeg"] = "image/jpeg";
    types["png"] = "image/png";
    types["gif"] = "image/gif";
    types["css"] = "text/css";
    types["js"] = "application/javascript";
    types["json"] = "application/json";
    types["pdf"] = "application/pdf";
    types["mp3"] = "audio/mpeg";
    types["mp4"] = "video/mp4";
    types["ico"] = "image/x-icon";
    types["svg"] = "image/svg+xml";
    types["default"] = "application/octet-stream";

}
void Response::setResponse(const std::string & str)
{
	this->httpResponse = str;  
}
Response::~Response()
{
}

std::string&    Response::getResponse() {return (httpResponse);}

