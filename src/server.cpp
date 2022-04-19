/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:51:18 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/19 05:48:48 by tenshi           ###   ########.fr       */
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
	// this->_rec = obj._rec;
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

int server::rec(int &socket, request& req)
{
	char				buff[1024];
	int					ret;

	ret = recv(socket, buff, sizeof(buff), 0);
	if (ret == -1)
	{
		std::cout << "recv() failed" << std::endl;
		return (-1);
	}
	if (ret == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		return (0);
	}
	std::string str(buff);
	// std::cout << str << std::endl;
	//! parse 
	// request *req = new request();
	// if (req->startParsing(str) < -1){
	// 	std::cout << "BAD REQUEST" << std::endl;
	// 	return -1;
	// }
	//! zedt dkchi li gal lya amine
	//! wlat 3ndi list fiha pair<socket, request>
	//! kat jik hna request li wslat l server f correct socket
	if (req.startParsing(str) < -1){
		std::cout << "BAD REQUEST" << std::endl;
		return -1;
	}
	return (1);
}

void server::handle_sockets(fd_set& fset)
{
	std::cout << "handle_sockets" << std::endl;
	std::list<std::pair<int, request> >::iterator socket = _sockets.begin();
	while (socket != _sockets.end())
	{
		if (FD_ISSET(socket->first, &fset))
		{
			int ret = rec(socket->first, socket->second);
			if (ret == 0)
			{
				FD_CLR(socket->first, &fset);
				close(socket->first);
				socket = _sockets.erase(socket);
				continue;
			}
			else if (ret == -1)
			{
				FD_CLR(socket->first, &fset);
				close(socket->first);
				socket =_sockets.erase(socket);
				continue;
			}
		}
		++socket;
	}
}

void server::add_socket(fd_set &fset, int &max_fd)
{
	std::cout << "search for new socket" << std::endl;
	std::cout << _sockets.size() << std::endl;
	if (FD_ISSET(_fd, &fset))
	{
		int sock = acc();
		if (sock != -1)
		{
			std::cout << "accepted()" << std::endl;
			FD_SET(sock, &fset);
			_sockets.push_back(std::make_pair(sock, request()));
			if (sock > max_fd)
				max_fd = sock;
		}
	}
}

bool server::is_sockets_empty(void) const
{
	return (_sockets.empty());
}