/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:13 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/18 00:18:25 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "socket.hpp"

/*---- Constructors & Destructor ------*/

Server::Server(){
}

Server::~Server(){
}

Server::Server(const Server &sv){
	*this = sv;
}

/*---- Operators -------*/

Server	&Server::operator=(const Server&obj){
	this->_socket = obj._socket;
	this->config = obj.config;
	return *this;
}

/*----- Accessors -------*/

std::vector<serverConfig*>	Server::getServers(void){
	return		config.getServers();
}

/*---- Member Functions ----*/

void		Server::conf(char **argv){
	config.parseFile(argv[1]);
}

void		Server::setup(void){
	std::vector<t_listen>	listen = config.getAllListenDir();
	size_t socket_nbr = 0;
	while (socket_nbr  < this->config.getServers().size())
	{
		//! add sockets
		std::cout << listen.at(0).port << " : " << listen.at(0).host << std::endl;
		socket_nbr++;
	}
}
