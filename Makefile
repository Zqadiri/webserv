# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/07 10:09:38 by zqadiri           #+#    #+#              #
#    Updated: 2022/05/29 14:44:28 by zqadiri          ###   ########.fr        #
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

.PHONY: 	all fclean clean re
