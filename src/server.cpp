/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:13 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/17 03:16:54 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*---- Constructors & Destructor ------*/

Server::Server(){
}

Server::Server(int domain, int type, int protocol, int port, u_long interface){
	int sok;
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);
	sok = socket(domain, type, protocol);
	if (sok < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	const int opt = 1;
	if (setsockopt(sok, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) < 0)){// * get and set options on sockets{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	//* set the socket to be a non-blocking socket
	int flag  = fcntl(sok, F_SETFL, O_NONBLOCK);
	if (flag == -1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
}

Server::~Server(){
	
}

Server::Server(const Server &sv){
	*this = sv;
}

/*---- Operators -------*/

Server	&Server::operator=(const Server&obj){
	this->_socket = obj._socket;
	this->_address = obj._address;
	this->_connection = obj._connection;
	this->config = obj.config;
	return *this;
}

/*----- Accessors -------*/

struct sockaddr_in Server::getAddress(void) const{
	return _address;
}

int Server::getConnection(void) const{
	return _connection;
}

// int	Server::getSocket(void) const{
// 	return _socket;
// }

std::vector<serverConfig*>	Server::getServers(void){
	return		config.getServers();
}

/*---- Member Functions ----*/

void	Server::testConnection(int sock){
	if (sock < 0){
		write (2, "Failed To Connect\n", 18);
		exit (EXIT_FAILURE);
	}
}

void    Server::conf(char **argv){
	config.parseFile(argv[1]);
}
