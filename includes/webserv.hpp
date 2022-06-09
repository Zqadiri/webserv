/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 13:51:02 by zqadiri           #+#    #+#             */
/*   Updated: 2022/06/09 16:56:46 by zqadiri          ###   ########.fr       */
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
#include <sys/stat.h>

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
#define BUFFER_SIZE	2048

#define OK								"HTTP/1.1 200 OK\r\n"
#define NO_CONTENT						"HTTP/1.1 204 No Content\r\n"
#define FORBIDDEN						"HTTP/1.1 403 Forbidden\r\n"
#define NOT_FOUND						"HTTP/1.1 404 Not Found\r\n"
#define	CREATED							"HTTP/1.1 201 Created\r\n"
#define MOVED_PERMANENTLY				"HTTP/1.1 301 Moved Permanently\r\n"
#define BAD_REQUEST						"HTTP/1.1 400 Bad Request\r\n"
#define INTERNAL_SERVER_ERROR			"HTTP/1.1 500 Internal Server Error\r\n"
#define METHOD_NOT_ALLOWED				"HTTP/1.1 405 Method Not Allowed\r\n"
#define PAYLOAD_TOO_LARGE				"HTTP/1.1 413 Payload Too Large\r\n"
#define HTTP_version_not_supported		"HTTP/1.1 505 HTTP Version Not Supported\r\n"
#define CONFLICT						"HTTP/1.1 409 Conflict\r\n"

#define URI_TOO_LONG					"HTTP/1.1 414 URI Too Long\r\n" //!NOT_IMPLEMENTED
#define NOT_IMPLEMENTED					"HTTP/1.1 501 Not Implemented\r\n"

typedef	std::vector<std::string>	configFile;

extern const char* keys[];
extern const char* locationKeys[];

static char **env_g;

typedef struct s_listen{
	unsigned int	host;
	int				port;
}   t_listen;

typedef struct s_redirect{
	int				code;
	std::string		path;
}	t_redirect;

typedef struct _error_pages{
	int				code;
	std::string		path;
}	t_error_pages;

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
