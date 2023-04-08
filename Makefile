# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 10:37:44 by ael-hayy          #+#    #+#              #
#    Updated: 2023/04/07 00:00:20 by aaitbelh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserve

FILES = Socket.cpp HandlGetRequest.cpp Client.cpp main.cpp Request.cpp Response.cpp Server.cpp 

OBJS = $(FILES:.cpp=.o)

CC = clang++
CFLAGS = -std=c++98 -g3


all:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME)

# clean:
# 	rm -f $(OBJS)
# fclean: clean
# 	rm -f $(NAME)
re :  all