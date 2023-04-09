/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandDeleteRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:37:36 by aaitbelh          #+#    #+#             */
/*   Updated: 2023/04/09 18:06:43 by aaitbelh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

void 			handlDeleteRequest(Client& client)
{
    std::ifstream file;
    std::string filename = client.getHeaderInfos()["URI"];
    file.open(filename);
    struct stat buffer;
    stat(filename.c_str(), &buffer);
    if((file.is_open()  && file.good()) && access(filename.c_str(), X_OK))
    {
        if(S_ISDIR(buffer.st_mode))
        {
            int len = 0;
            DIR *dir = NULL;
            dir = opendir(filename.c_str());
            if(!dir)
                exit(204);
            struct dirent *ent = NULL;
            while((ent = readdir(dir)) != NULL)
            {
                if(ent->d_name[0] != '.')
                    len++;
            }
            if(len)
                exit(409);
            rmdir(filename.c_str());
        }
        else
            remove(filename.c_str());
        exit(200);
    }
    else
        exit(204); 
}