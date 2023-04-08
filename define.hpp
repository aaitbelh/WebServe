/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hayy <ael-hayy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:11:52 by ael-hayy          #+#    #+#             */
/*   Updated: 2023/03/30 02:55:15 by ael-hayy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP


//* DEFINING ...


#define     POST_REQ    0
#define     GET_REQ     1
#define     DELETE_REQ  2


//status 1 for finished body and 0 for not finished
#define     FINISHED_BODY 		1
#define     NOT_FINISHED_BODY	0

#define     BUFFER_SIZE 		1024


#define     uploadsPath			"/Users/ael-hayy/Desktop/webb/uploads"
//* TYPEDEF ....
typedef struct sockaddr_storage SOCKADDRE;

typedef     int         REQUES_TYPE;

typedef     int         SOCKET;

#endif