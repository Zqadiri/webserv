# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/07 10:09:38 by zqadiri           #+#    #+#              #
#    Updated: 2022/06/03 12:12:26 by zqadiri          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

SRCS		=	./src/config.cpp \
				./src/server.cpp \
				./src/serverConfig.cpp \
				./src/servers.cpp \
				./src/Request/request.cpp \
				./Response/Response.cpp \
				./Response/MimeTypes.cpp \
				./src/Request/requestParser.cpp \
				./src/CGI/CGI.cpp \
				./src/utils/utils.cpp \
				webserv.cpp

OBJS		= $(SRCS:.cpp=.o)

CC			= c++

FLAGS		=  -g -std=c++98

WWW			= -Wall -Wextra -Werror

# WWW = -fsanitize=address -g

all:		$(NAME)

$(NAME):	$(SRCS)
			@ $(CC) $(FLAGS) $(WWW) $(SRCS) -o $(NAME)

clean:		
			@ rm -f $(OBJS)

fclean:		clean
			@ rm -f $(NAME)

re:			fclean $(NAME)
			@rm -rf ./tmp/*

.PHONY: 	all fclean clean re
