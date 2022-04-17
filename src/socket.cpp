/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 12:19:59 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/15 21:38:41 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

/*----- Constructors -------*/

Socket::Socket(){
}

Socket::Socket(int domain, int type, int protocol, int port, u_long interface){
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

Socket::~Socket(){
}

Socket::Socket(const Socket &obj){
	*this = obj;
}

Socket	&Socket::operator=(const Socket&obj){
	this->_socket = obj._socket;
	this->_address = obj._address;
	this->_connection = obj._connection;
	return *this;
}

/*----- Accessors -------*/

struct sockaddr_in Socket::getAddress(void) const{
	return _address;
}

int Socket::getConnection(void) const{
	return _connection;
}

// int	Socket::getSocket(void) const{
// 	return _socket;
// }

/*------ Member Functions--------*/

void	Socket::testConnection(int sock)
{
	if (sock < 0){
		write (2, "Failed To Connect\n", 18);
		exit (EXIT_FAILURE);
	}
}