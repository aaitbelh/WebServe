/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:39:51 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/04/10 05:26:31 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

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
        std::string path = client.getHeaderInfos()["URI"];
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
        }
        closedir(dir);
        return body.length();
}
void Response::fillTheHeader(Client &client)
{
    std::map<std::string, std::string> &request = client.getRequest().getHeaderInfos();
    std::string filename = request["URI"];
    // filename.erase(0, 1);
    struct stat buffer;
    std::stringstream s;
    stat(filename.c_str(), &buffer);
    client.file.open(filename);
    if(client.file.good() && S_ISDIR(buffer.st_mode))
    {
        header = "HTTP/1.1 200 OK\r\n";   
        filename = "listing-dir.html";
        client.is_dir = 1;
        client.file.close();
        client.file.clear();
        client.file.open("listing-dir.html");
    }
    else
    {
        if(client.file.is_open())
        {
            header = "HTTP/1.1 200 OK\r\n";
        }
        else
        {
            header = "HTTP/1.1 404 KO\r\n";
            filename  = "public/404.html";
            client.file.close();
            client.file.clear();
            client.file.open(filename);
        }
    }
    stat(filename.c_str(), &buffer);
    header.append("Connection: close\r\n");
    std::string date;
    time_t  now = time(0);
    struct tm *t = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", t);
    date = buf;
    header.append("Date: " + date + "\r\n");
    if(client.is_dir)
    {
        s <<  buffer.st_size + calcluateLen(request["URI"]);    
    }
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
