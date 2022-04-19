/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:51:18 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/19 02:55:20 by nwakour          ###   ########.fr       */
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
	this->_sockets = obj._sockets;
	this->_rec = obj._rec;
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
	int socket = accept(_fd, NULL, NULL);
	if (socket == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return (-1);
	}
	return socket;
}

int server::get_fd(void) const
{
	return (_fd);
}

void server::add_socket(int &socket)
{
	_sockets.push_back(socket);
}

int server::rec(int &socket)
{
	char				buff[1024];
	int					ret;

	ret = recv(socket, buff, sizeof(buff), 0);
	if (ret == -1)
	{
		close(socket);
		std::cout << "recv() failed" << std::endl;
		return (-1);
	}
	if (ret == 0)
	{
		close(socket);
		std::cout << "Client disconnected" << std::endl;
		return (0);
	}
	_rec.append(buff);
	return (1);
}

void server::print_rec(void)
{
	std::cout << _rec << std::endl;
	_rec.clear();
}

int server::handle_sockets(fd_set& fset)
{
	for (std::list<int>::iterator socket = _sockets.begin(); socket != _sockets.end(); ++socket)
	{
		if (FD_ISSET(*socket, &fset))
		{
			int ret = rec(*socket);
			if (ret == 0)
			{
				print_rec();
				FD_CLR(*socket, &fset);
				_sockets.erase(socket);
				std::cout << "done and waiting for response" << std::endl;
				break;
			}
			else if (ret == -1)
			{
				std::cout << "error rec()" << std::endl;
				FD_CLR(*socket, &fset);
				_sockets.erase(socket);
				break;
			}
		}
	}
}