/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:13 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/06 18:10:43 by zqadiri          ###   ########.fr       */
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
	_max_fd = 0;
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
	
	std::cout << "run()\n";
	struct timeval		tv;
	fd_set write_set;
	FD_ZERO(&write_set);
	
	while (1)
	{
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		int selected = 0;
		std::cout << "select()\n";
		while (selected == 0)
		{
			fd_set fset = _fd_set;
			fd_set wset = write_set;
			
			selected = select(_max_fd + 1, &fset, &wset, NULL, &tv);
			if (selected == -1)
			{
				std::cout << "select error" << std::endl;
				return ;
			}
		}
		if (selected > 0)
		{
			for (std::list<server>::iterator serv = _servers.begin(); serv != _servers.end(); ++serv){
				if (!serv->is_sockets_empty())
				{
					serv->handle_sockets(_fd_set, write_set);
					//!mine 
					this->selectServer();
					//!
				}
				else
					serv->add_socket(_fd_set, _max_fd);
			}
		}
		else
			std::cout << "didnt select" << std::endl;
	}
}

int		Servers::selectServer(void)
{
	// std::list<int> possibleMatch;
	// // ? FISRT : Parsing the listen Directive to Find Possible Matches
	// std::vector<t_listen>	listen = config.getAllListenDir();
	// std::list<std::list<std::string> > servNames = config.getAllServerNames();
	// for (size_t i = 0; i < listen.size(); i++)
	// 	std::cout << listen.at(i).host << " - " << listen.at(i).port << std::endl;
	// for (std::list<std::list<std::string> >::iterator i = servNames.begin(); 
	// 				i != servNames.end(); ++i)
	// {
	//     std::list<std::string> inerList = *i;
	// 	for (std::list<std::string>::iterator it = inerList.begin(); 
	// 				it != inerList.end(); ++it)
	// 		std::cout << " > " << *it << std::endl;
	// 	std::cout << " ------- " << std::endl;
	// }

	// for (std::list<server>::iterator i = this->_servers.begin();
	// 				i != this->_servers.end(); ++i){
	// 	server s = *i;
	// 	std::list<std::pair<int, request> > v = s.getRequest();
	// 	for (std::list<std::pair<int, request> >::iterator i = v.begin();
	// 			i != v.end(); ++i)
	// 	{
	// 		//! print vector data	
	// 		// i->second.getHost();
	// 	}
	// }
	return 0;
}