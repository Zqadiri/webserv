/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:51:18 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/18 23:23:03 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../includes/webserv.hpp"

server::server(){
	
}
server::~server(){

}
server::server(const server &sv){
	*this = sv;
}

server &server::operator=(const server &obj){
	this->_addr = obj._addr;
	this->_fd = obj._fd;
	this->_port = obj._port;
	this->_host = obj._host;
	this->_socket = obj._socket;
	this->_rec = obj._rec;
	return *this;
}


server::server(t_listen &l) :  _port(l.port),_host(l.host), _fd(-1), _socket(-1){
	bzero((char *)&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(_host);
	_addr.sin_port = htons(_port);
	setup();
}

int server::setup(void)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
	{
		std::cout << "socket() failed" << std::endl;
		return (-1);
	}
	if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
	{
		std::cout << "bind() failed" << std::endl;
		return (-1);
	}
	if (listen(_fd, 10) == -1)
	{
		std::cout << "listen() failed" << std::endl;
		return (-1);
	}
	return (1);
}

int server::acc(void)
{
	_socket = accept(_fd, NULL, NULL);
	if (_socket == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return (-1);
	}
	return _socket;
}

int server::get_fd(void) const
{
	return (_fd);
}
int server::get_socket(void) const
{
	return (_socket);
}

void server::set_socket(int socket)
{
	_socket = socket;
}

int server::rec(void)
{
	char				buff[1024];
	int					ret;

	ret = recv(_socket, buff, sizeof(buff), 0);
	if (ret == -1)
		return (-1);
	if (ret == 0)
		return (0);
	_rec.append(buff);
	return (1);
}

void server::print_rec(void)
{
	std::cout << _rec << std::endl;
}
