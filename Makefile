# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/07 10:09:38 by zqadiri           #+#    #+#              #
#    Updated: 2022/05/06 18:09:05 by zqadiri          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

SRCS		=  	webserv.cpp ./src/*.cpp ./src/Request/*.cpp

OBJS		= $(SRCS:.cpp=.o)

CC			= c++

FLAGS		=  -std=c++98

# WWW			= -Wall -Wextra -Werror

all:		$(NAME)

$(NAME):	$(SRCS)
			@ $(CC) $(FLAGS) $(WWW) $(SRCS) -o $(NAME)

clean:		
			@ rm -f $(OBJS)

fclean:		clean
			@ rm -f $(NAME)

re:			fclean $(NAME)

.PHONY: 		all fclean clean re
