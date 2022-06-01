/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:51:18 by nwakour           #+#    #+#             */
/*   Updated: 2022/06/01 11:32:35 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"
#include "../includes/webserv.hpp"
#include "../Response/Response.hpp"

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
	this->_sockets = obj._sockets;
	this->_config = obj._config;
	return *this;
}


server::server(t_listen &l, serverConfig* conf) : _fd(-1), _config(conf){
	bzero((char *)&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(l.host);
	_addr.sin_port = htons(l.port);
}

int server::setup(void)
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
	{
		std::cout << "socket() failed" << std::endl;
		return (-1);
	}
	int one = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		return (-1);
	}
	if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
	{
		std::cout << "bind() failed" << std::endl;
		return (-1);
	}
	if (listen(_fd, 20) == -1)
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

int server::get_fd(void) const{
	return (_fd);
}

int server::rec(int &socket, request& req)
{
	char				buff[BUFFER_SIZE];
	int					ret;
	
	std::cout << "trying recv from " << socket << "\n";
	ret = recv(socket, buff, sizeof(buff) - 1 , 0);
	if (ret == -1)
	{
		std::cout << "recv() failed" << std::endl;
		return (-1);
	}
	if (ret == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		return (-1);
	}
	buff[ret] = '\0';
	std::string str(buff, ret);
	int ret_parse = req.parseRquest(str, req, socket);
	std::cout << YELLOW <<  buff << RESET << std::endl;
	// std::cout << "{ret} " <<  ret_parse << std::endl;
	if (ret_parse < -1){ //! return -1 if the content length header is absent
		std::cout << "BAD REQUEST" << std::endl;
		return -1;
	}
	return (ret_parse);
}

void server::handle_sockets(fd_set &cp_fset, fd_set &cp_wset, fd_set& fset, fd_set& wset)
{
	std::list<std::pair<int, request> >::iterator socket = _sockets.begin();
	while (socket != _sockets.end())
	{
		if (FD_ISSET(socket->first, &cp_wset))
		{
			
			int ret = sen(socket->first, socket->second, _responses[socket->first]);
			if (ret == -1)
			{
				std::cout << "send() failed" << std::endl;
				FD_CLR(socket->first, &wset);
				FD_CLR(socket->first, &fset);
				
				close(socket->first);
				socket =_sockets.erase(socket);
				_responses.erase(socket->first);
				// break ;
			}
			else if (ret == 0)
			{
				FD_CLR(socket->first, &wset);
				if (socket->second.getContentFromMap("Connection").compare("keep-alive"))
				{
					close(socket->first);
					socket =_sockets.erase(socket);
					std::cout << "close socket" << std::endl;
				}
				else
				{
					FD_SET(socket->first, &fset);
					++socket;
					std::cout << "keep socket alive" << std::endl;
				}
				_responses.erase(socket->first);
			}
			else
				++socket;
		}
		else
			++socket;
	}
	socket = _sockets.begin();
	while (socket != _sockets.end())
	{
		if (FD_ISSET(socket->first, &cp_fset))
		{
			int ret = rec(socket->first, socket->second);
			if (ret == -1)
			{
				FD_CLR(socket->first, &fset);
				close(socket->first);
				socket =_sockets.erase(socket);
				// break;
			}
			else if (ret == 0)
			{
				std::cout << "recv() success" << std::endl;
				FD_CLR(socket->first, &fset);
				FD_SET(socket->first, &wset);
				_responses[socket->first] = Response(socket->first);
				++socket;
			}
			else
				++socket;
		}
		else
			++socket;
	}
}

int server::add_socket(fd_set &cp_fset,fd_set &fset, int &max_fd)
{
	if (FD_ISSET(_fd, &cp_fset))
	{
		int sock = acc();
		if (sock != -1)
		{
			if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
			{
				std::cout << "fcntl() failed" << std::endl;
				return (-1);
			}
			std::cout << sock << " accepted" << std::endl;
			FD_SET(sock, &fset);
			_sockets.push_back(std::make_pair(sock, request(sock)));
			if (sock > max_fd)
				max_fd = sock;
			return (0);
		}
	}
	return (1);
}

bool server::is_sockets_empty(void) const{
	return (_sockets.empty());
}

void 	server::check_timeout(fd_set& fdset, const std::time_t& current_time)
{
	std::list<std::pair<int, request> >::iterator socket = _sockets.begin();
	while (socket != _sockets.end())
	{
		if (current_time - socket->second.getTime() > TIMEOUT)
		{
			std::cout << "timeout" << std::endl;
			FD_CLR(socket->first, &fdset);
			close(socket->first);
			socket = _sockets.erase(socket);
		}
		else
			++socket;
	}
}
std::list<std::pair<int, request> >		server::getRequest(void){
	return this->_sockets;
}

void string_to_char(std::string str, char *s)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		s[i] = str[i];
	}
	s[str.size()] = '\0';
}
int server::sen(int &socket, request& req, Response &response)
{
	char			buff[BUFFER_SIZE];
	std::string		myline;
	int size = 0;
	int				ret = 0;
	bool over = false;
	int digits = 0;
	std::cout << "trying send to " << socket << "\n";
	
	response.Return_string(req, _config, socket);
	if (response.body_length > 0)
	{
		if (!response._res.is_open())
			response._res.open(response.str_uri, std::fstream::in);
		if(!response._res.is_open()){
			std::cout << "open() failed !!!!" << std::endl;
			return (-1);
		}
		if (!response.chunked)
		{
			string_to_char(response.header, buff);
			size = response.header.size();
			response._res.read(buff + size, response.body_length);
			size += response.body_length;
			response._res.close();
			over = true;
		}
		else
		{
			if (!response.header.empty())
			{
				string_to_char(response.header, buff);
				size = response.header.size();
				response.header.clear();
			}
			{
				std::string d;
				d += response.body_length;
				digits = d.size();
			}
			if (response.body_length > BUFFER_SIZE - (size + 8))
			{
				std::string d;
				d += BUFFER_SIZE - (size + 8);
				digits = d.size();
				string_to_char(d, buff + size);
				size += digits;
				buff[size++] = '\r';
				buff[size++] = '\n';
				response._res.read(buff + size, BUFFER_SIZE - (size + digits + 2));
				response.body_length -= BUFFER_SIZE - (size + digits + 2);
				buff[size++] = '\r';
				buff[size++] = '\n';
			}
			else
			{
				std::string d;
				d += response.body_length;
				digits = d.size();
				string_to_char(d, buff + size);
				size += digits;
				buff[size++] = '\r';
				buff[size++] = '\n';
				response._res.read(buff + size, response.body_length);
				if (response.body_length > 0)
					response.body_length = 0;
				else
				{
					response._res.close();
					over = true;
				}
				buff[size++] = '\r';
				buff[size++] = '\n';
			}
		}
	}
	else
	{
		string_to_char(response.header, buff);
		size = response.header.size();
	}
	
	std::cout << YELLOW << ">" << buff << "<" << RESET << std::endl;
	ret = send(socket, buff, size, 0);
	if (ret == -1){
		std::cout << "send() failed !!!!" << std::endl;
		return (-1);
	}
	if (over)
		return (0);
	return (ret);
}