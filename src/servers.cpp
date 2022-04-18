/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:13 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/18 23:29:25 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servers.hpp"
#include "socket.hpp"

/*---- Constructors & Destructor ------*/

Servers::Servers(){
	
}

Servers::~Servers(){
}

Servers::Servers(const Servers &sv){
	*this = sv;
}

/*---- Operators -------*/

Servers	&Servers::operator=(const Servers&obj){
	this->config = obj.config;
	return *this;
}

/*----- Accessors -------*/

std::vector<serverConfig*>	Servers::getServers(void){
	return		config.getServers();
}

/*---- Member Functions ----*/

// void	Servers::testConnection(int sock){
// 	if (sock < 0){
// 		write (2, "Failed To Connect\n", 18);
// 		exit (EXIT_FAILURE);
// 	}
// }

void		Servers::conf(char **argv){
	config.parseFile(argv[1]);
}

void		Servers::setup(void){
	std::vector<t_listen>	listen = config.getAllListenDir();
	
	FD_ZERO(&_fd_set);
	for(std::vector<t_listen>::iterator it = listen.begin(); it != listen.end(); ++it){
		server sev(*it);
		int fd = sev.get_fd();
		FD_SET(fd, &_fd_set);
		_servers[fd] = sev;
		std::cout << "Server: "<< fd << " " << it->port << " Setup Done" << std::endl;
	}
	
}

void		Servers::run(void){
	struct timeval	timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	int max_fd = ((--_servers.end())->first);
	while (1)
	{
		int selected = 0;
		std::cout << "Waiting for connections..." << std::endl;
		while (selected == 0)
		{
			fd_set fd_set = _fd_set;
			std::cout << "selet()" << std::endl;
			selected = select(max_fd + 1, &fd_set, NULL, NULL, &timeout);
			if (selected == -1)
			{
				std::cout << "select error" << std::endl;
				return ;
			}
		}
		for (std::map<int ,server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
		{
			int socket = it->second.get_socket();
			if (socket != -1)
			{
				if (FD_ISSET(socket, &_fd_set))
				{
					int ret = it->second.rec();
					if (ret == 0)
					{
						it->second.print_rec();
						FD_CLR(socket, &_fd_set);
						std::cout << "done and waiting for response" << std::endl;
					}
					else if (ret == -1)
					{
						std::cout << "error rec()" << std::endl;
						FD_CLR(socket, &_fd_set);
						it->second.set_socket(-1);
					}
				}
			}
			else
			{
				if (FD_ISSET(it->first, &_fd_set))
				{
					std::cout << "accepted()" << std::endl;
					int sock = it->second.acc();
					if (sock != -1)
						FD_SET(sock, &_fd_set);
					if (sock > max_fd)
						max_fd = sock;
				}
			}
		}
	}
}