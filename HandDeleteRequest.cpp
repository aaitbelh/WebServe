/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandDeleteRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:37:36 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/04/10 05:30:07 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

void 			handlDeleteRequest(Client& client)
{
    std::ifstream file;
    std::string filename = client.getHeaderInfos()["URI"];
    filename.erase(0, 1);
    file.open(filename);
    struct stat buffer;
    if((file.is_open() && file.good()) && !access(filename.c_str(), W_OK))
    {
        if(S_ISDIR(buffer.st_mode))
        {
            int len = 0;
            DIR *dir = NULL;
            dir = opendir(filename.c_str());
            if(!dir)
                sendResponse(404, client);
            struct dirent *ent = NULL;
            while((ent = readdir(dir)) != NULL)
            {
                if(ent->d_name[0] != '.')
                    len++;
            }
            if(len)
                sendResponse(204, client);
            rmdir(filename.c_str());
        }
        else
            remove(filename.c_str());
        sendResponse(204, client);
    }
    else
        sendResponse(404, client);
}