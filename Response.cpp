/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:51 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/05/01 13:29:38 by aaitbelh         ###   ########.fr       */
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
        DIR *dir = NULL;
        std::string path = client.dirname;
        struct dirent *ent = NULL;
        if ((dir = opendir(path.c_str())) != NULL) {
            // path.append("/");
        while ((ent = readdir (dir)) != NULL) {
                if(ent->d_name[0] != '.')
                {
                    std::string filename = ent->d_name;
                    std::string str = "\n<li><a href=\"" + path + filename + "\">" + filename + "</a></li>";
                    this->body.append(str);
                }
        }
        this->body.append("\n</ul></body></html>");
        }
        closedir(dir);
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
int calcluateLen(std::string  path)
{ 
    std::string body;
    DIR *dir = NULL;
	std::cout << path << std::endl;
    struct dirent *ent = NULL;
    if ((dir = opendir(path.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] != '.')
            {
                std::string filename = ent->d_name;
                std::string str = "\n<li><a href=\"" + path + "/" + filename + "\">" + filename + "</a></li>";
                body.append(str);  
            }
    }
    	body.append("\n</ul></body></html>");
		closedir(dir);
    }
    return body.length();
}
std::string Response::find_filename(Client &client)
{
	std::string filename;
	std::ifstream file;
	for(std::list<std::string>::iterator it = client.GetClientinfos().index_files.begin(); it !=  client.GetClientinfos().index_files.end(); ++it)
	{
		std::string path =  client.GetClientinfos().root + client.GetClientinfos().location_div.location_path + "/"+ *it;
		file.open(path);
		if(file.is_open() && file.good())
			return path;
	}
	return client.GetClientinfos().root + "/" + client.getHeaderInfos()["URI"];
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
void Response::fillTheHeader(Client &client)
{
	checkRediraction(client);
    std::map<std::string, std::string> &request = client.getRequest().getHeaderInfos();
    std::string filename = find_filename(client);
	std::cout << "-------->"  << filename << std::endl;
    struct stat buffer;
    std::stringstream s;
    stat(filename.c_str(), &buffer);
    client.file.open(filename);
	this->header = request["VERSION"];
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
        if(client.file.is_open())
            header += " 200 OK\r\n";
        else
        {
			// sendResponse(404, client);
            header += " 404 KO\r\n";
            filename  = "public/404.html";
            client.file.close();
            client.file.clear();
            client.file.open(filename);
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
        s <<  buffer.st_size + calcluateLen(client.dirname);    
    else
        s << buffer.st_size;
    header.append("Content-Length: " + s.str() + "\r\n");
    header.append("Content-Type: " + getFileType(filename) + "\r\n");
    header.append("\r\n");
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
