/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:43:30 by ael-hayy          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/04/07 00:11:15 by aaitbelh         ###   ########.fr       */
=======
/*   Updated: 2023/03/30 03:09:34 by ael-hayy         ###   ########.fr       */
>>>>>>> 1819d1c75ef2c3d2b1ebb0031f9a7353cc958c07
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"

int main()
{
    Socket  socketListen("0", "8080");
    if (socketListen() < 0)
        return (-1);
    std::list<Client>   clientList;
    // int l = 0;
    signal(SIGPIPE, SIG_IGN);
    while (true)
    {
        fd_set  readSet;
        fd_set  writeSet;
        setSocketForReadAndWrite(&readSet, &writeSet, socketListen());
        if (waitingForClients(&readSet, &writeSet, socketListen(), clientList) < 0)
            return (-1);
        acceptNewConnictions(&readSet, &writeSet, socketListen(), clientList);
        // std::cout<<"Clinet list size: "<<clientList.size()<<std::endl;
        std::list<Client>::iterator i = clientList.begin(), j;
        j = i;
        while (i != clientList.end())
        {
            ++j;
            acceptREADsocket(&readSet,&writeSet, *i, clientList, i);
            i = j;
        }
        
    }
    return (0);
}