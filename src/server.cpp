/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:51:18 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/18 16:44:21 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "../includes/webserv.hpp"

server::server(){
	
}
server::~server(){
	if (_fd > 0)
		close(_fd);
}
server::server(const server &sv){
	*this = sv;
}

server &server::operator=(const server &obj){
	this->_addr = obj._addr;
	this->_fd = obj._fd;
	this->_port = obj._port;
	this->_host = obj._host;
	return *this;
}


server::server(t_listen &l) :  _port(l.port),_host(l.host), _fd(-1){
	bzero((char *)&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(_host);
	_addr.sin_port = htons(_port);
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
	int s;

	s = accept(_fd, NULL, NULL);
	if (s == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return (-1);
	}
	return s;
}

int server::get_fd(void) const
{
	return (_fd);
}