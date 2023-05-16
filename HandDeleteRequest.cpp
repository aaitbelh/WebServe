/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandDeleteRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:37:36 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/05/16 08:51:36 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

void 			handlDeleteRequest(Client& client)
{
    std::ifstream file;
    std::string filename = find_filename(client);
    int Rvalue = 0;
    file.open(filename);
    struct stat buffer;
    stat(filename.c_str(), &buffer);
    if(((file.is_open() && file.good()) && !access(filename.c_str(), W_OK)) && !filename.empty())
    {
        if(S_ISDIR(buffer.st_mode))
        {
            int len = 0;
            DIR *dir = NULL;
            dir = opendir(filename.c_str());
            if(!dir)
            {
                if(!client.server.error_page.count(404))
                    sendResponse(404, client);
                setInfos_header(client, client.server.error_page[404], &Rvalue);   
                changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 404 Not Found");
                if(Rvalue)
                    sendResponse(404, client);
                return ;
            }
            struct dirent *ent = NULL;
            while((ent = readdir(dir)) != NULL)
            {
                if(ent->d_name[0] != '.')
                    len++;
            }
            if(len)
            {
                if(!client.server.error_page.count(409))
                    sendResponse(409, client);
                client.getRes().getHeader() = setInfos_header(client, client.server.error_page[409], &Rvalue);
                if(Rvalue)
                    sendResponse(409, client);
                changeTheHeaderby(client, client.getHeaderInfos()["VERSION"] + " 409 Conflict");
                return ;
            }
            rmdir(filename.c_str());
        }
        else
            remove(filename.c_str());    
        sendResponse(204, client);
    }
    else
    {
        if(!client.server.error_page.count(404))
            sendResponse(404, client);
        setInfos_header(client, client.server.error_page[404], &Rvalue);
        if(Rvalue)
            sendResponse(404, client);
    }
}