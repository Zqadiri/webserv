/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:51:18 by nwakour           #+#    #+#             */
/*   Updated: 2022/06/09 21:23:51 by nwakour          ###   ########.fr       */
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
	this->_port = obj._port;
	return *this;
}


server::server(t_listen &l, serverConfig* conf) : _port(l.port), _fd(-1){

	std::vector<std::string> server_names = conf->getServerName();

	bzero((char *)&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(l.host);
	_addr.sin_port = htons(l.port);
	for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); ++it){
		_config.push_back(std::make_pair(*it, conf));
	}
	if (server_names.empty())
		_config.push_back(std::make_pair("", conf));
}

int		server::get_port(void) const
{
	return (_port);
}

void server::add_servconf(serverConfig* conf){
	std::vector<std::string> server_names = conf->getServerName();
	for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); ++it){
		_config.push_back(std::make_pair(*it, conf));
	}
	if (server_names.empty())
		_config.push_back(std::make_pair("", conf));
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
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) == -1)
	{
		std::cout << "setsockopt() failed" << std::endl;
		return (-1);
	}
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
	if (listen(_fd, 1024) == -1)
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
	if (fcntl(socket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		return (-1);
	}
	return socket;
}

int server::get_fd(void) const{
	return (_fd);
}

int server::rec(int &socket, request& req)
{
	char				buff[BUFFER_SIZE + 1];
	int					ret;
	
	std::cout << "trying recv from " << socket << "\n";
	ret = recv(socket, buff, BUFFER_SIZE , 0);
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
	std::cout<< GREEN << buff << RESET << std::endl;
	int ret_parse = req.parseRquest(str, req, socket);
	std::cout << "{ret} " <<  ret_parse << std::endl;
	std::cout << "{code} " <<  req.getRetCode() << std::endl;
	if (ret_parse < 0){
		std::cout << "BAD REQUEST" << std::endl;
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
			std::map<int, Response>::iterator resp = _responses.find(socket->first);
			if (resp == _responses.end())
			{
				FD_CLR(socket->first, &wset);
				close(socket->first);
				socket =_sockets.erase(socket);
			}
			else
			{
				int ret = sen(socket->first, socket->second, resp->second);
				if (ret == -1)
				{
					std::cout << "send() failed" << std::endl;
					FD_CLR(socket->first, &wset);
					close(socket->first);
					if (resp->second.getRes().is_open())
						resp->second.getRes().close();
					_responses.erase(resp);
					socket =_sockets.erase(socket);
				}
				else if (ret == 0)
				{
					FD_CLR(socket->first, &wset);
					if (resp->second.getRes().is_open())
						resp->second.getRes().close();
					if (socket->second.getContentFromMap("Connection").compare("keep-alive"))
					{
						close(socket->first);
						_responses.erase(resp);
						socket =_sockets.erase(socket);
						std::cout << "close socket" << std::endl;
					}
					else
					{
						int sock = socket->first;
						FD_SET(sock, &fset);
						_responses.erase(resp);
						socket =_sockets.erase(socket);
						socket = _sockets.insert(socket, std::make_pair(sock, request(sock)));
						++socket;
						std::cout << "keep socket alive" << std::endl;
					}
					
				}
				else
				{
					std::cout << "not done !!" << std::endl;
					++socket;
				}
			}
		}
		else if (FD_ISSET(socket->first, &cp_fset))
		{
			int ret = rec(socket->first, socket->second);
			if (ret == -1)
			{
				FD_CLR(socket->first, &fset);
				close(socket->first);
				socket =_sockets.erase(socket);
			}
			else if (ret == 0)
			{
				std::cout << "recv() success" << std::endl;
				FD_CLR(socket->first, &fset);
				FD_SET(socket->first, &wset);
				std::cout << RED << "new response" << RESET << std::endl;
				_responses.insert(std::make_pair(socket->first, Response(socket->first)));
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

void 	server::check_timeout(fd_set& fdset, const std::time_t& current_time, fd_set &write)
{
	std::list<std::pair<int, request> >::iterator socket = _sockets.begin();
	while (socket != _sockets.end())
	{
		if (current_time - socket->second.getTime() > TIMEOUT)
		{
			std::cout << "timeout" << std::endl;
			FD_CLR(socket->first, &fdset);
			FD_CLR(socket->first, &write);
			close(socket->first);
			_responses.erase(socket->first);
			socket->second.closeStream();
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
	char			buff[BUFFER_SIZE + 1];

	std::string		myline;
	int size = 0;
	int				ret = 0;
	bool over = false;
	std::cout << "trying send to " << socket << "\n";
	req.reset_timer();
	if (response.get_handled() == false)
	{
		std::string host = req.getHost();
		serverConfig* config = _config.begin()->second;
		if (!host.empty())
		{
			for (std::list<std::pair<std::string, serverConfig*> >::iterator it = _config.begin(); it != _config.end(); ++it)
			{
				if (it->first.compare(host) == 0)
				{
					config = it->second;
				}
			}
		}
		response.Return_string(req, config, socket);
	}

	std::cout << RED << "body lenght = "  << response.get_body_length() << RESET << std::endl;
	if (response.get_body_length() > 0)
	{
		std::cout << response.get_str_uri() << std::endl;
		if (!response.getRes().is_open())
		{
			std::cout << "open()" << std::endl;
			response.getRes().open(response.get_str_uri(), std::ifstream::in | std::ifstream::binary);
		}
		if(!response.getRes().is_open()){
			std::cout << "open() failed !!!!" << std::endl;
			return (-1);
		}
		if (!response.get_header().empty())
		{
			string_to_char(response.get_header(), buff);
			size = response.get_header().size();
			response.get_header().clear();
		}
		response.getRes().read(buff + size, (BUFFER_SIZE - size));
		response.set_body_length(response.get_body_length() - response.getRes().gcount());
		size +=  response.getRes().gcount();
		if (response.get_body_length() == 0)
			over = true;
	}
	else
	{
		string_to_char(response.get_header(), buff);
		size = response.get_header().size();
		over = true;
	}
	buff[size] = '\0';
	
	std::cout << YELLOW << ">" << buff << "<" << RESET << std::endl;
	// std::cout << "*********** "  << socket <<  " "<< size << std::endl;
	ret = send(socket, buff, size, 0);
	// std::cout << "***********" << std::endl;

	if (ret == -1){
		std::cout << "send() failed !!!!" << std::endl;
		return (-1);
	}
	std::cout << "send() " << ret << std::endl;
	if (over)
		return (0);
	return (ret);
}