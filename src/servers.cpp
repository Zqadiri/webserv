/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:13 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/19 05:43:44 by tenshi           ###   ########.fr       */
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
		if (sev.setup() != -1)
		{
			int fd = sev.get_fd();
			FD_SET(fd, &_fd_set);
			if (fd > _max_fd)
				_max_fd = fd;
			_servers.push_back(sev);
			std::cout << "Server: "<< fd << " " << it->port << " Setup Done" << std::endl;
		}
		else
			std::cout << "Server: "<< it->port << " Setup Failed" << std::endl;
	}
}

void		Servers::run(void){
	
	struct timeval	timeout;
	while (1)
	{
		std::cout << "run()\n";
		int selected = 0;
		while (selected == 0)
		{
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			fd_set fset = _fd_set;
			std::cout << "select()\n";
			selected = select(_max_fd + 1, &fset, NULL, NULL, &timeout);
			if (selected == -1)
			{
				std::cout << "select error" << std::endl;
				return ;
			}
		}
		if (selected > 0)
		{
			for (std::list<server>::iterator serv = _servers.begin(); serv != _servers.end(); ++serv)
			{
				if (!serv->is_sockets_empty())
					serv->handle_sockets(_fd_set);
				else
					serv->add_socket(_fd_set, _max_fd);
			}
		}
		else
			std::cout << "didnt select" << std::endl;
	}
}