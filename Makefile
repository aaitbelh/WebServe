# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaitbelh <aaitbelh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 10:37:44 by ael-hayy          #+#    #+#              #
#    Updated: 2023/05/01 21:18:17 by aaitbelh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserve

FILES = Socket.cpp HandlGetRequest.cpp Client.cpp Request.cpp Response.cpp Server.cpp HandDeleteRequest.cpp pars/parsing.cpp

OBJS = $(FILES:.cpp=.o)

CC = clang++
CFLAGS = -std=c++98 -g3 -fsanitize=address
all: $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
# all:
# 	$(CC) $(CFLAGS) $(FILES) -o $(NAME)
	
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re :  fclean all