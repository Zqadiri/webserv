/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 13:51:02 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/15 16:45:01 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

// CPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <list>
#include <iterator>
#include <sstream>
#include <algorithm>

// C 
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

// Network 
#include <sys/socket.h>
#include <netinet/in.h>

// Colors
# define RED "\033[31m"
# define GREEN "\033[32m"
# define OR "\033[34m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"

#define TIMEOUT 20


typedef	std::vector<std::string>	configFile;

extern const char* keys[];
extern const char* locationKeys[];

typedef struct s_listen{
	unsigned int	host;
	int				port;
}   t_listen;

//* Request
enum ParseStatus {
	HEADERS,
	BODY,
	CHUNK,
	COMPLETE
};

enum ChunkStatus {
	CHUNK_BODY,
	CHUNK_SIZE,
};

#endif
