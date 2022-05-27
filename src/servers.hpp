/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:10 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/27 17:23:48 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
#define SERVERS_HPP

#include "../includes/webserv.hpp"
#include "config.hpp"
#include "server.hpp"

class server;

class Servers
{
	private:
		Config					config;
		std::list<server>		_servers;
		int						_max_fd;
		fd_set					_fd_set;
		
	public:
		Servers(void);
		~Servers();
		Servers(const Servers&);
		Servers	&operator=(const Servers&);

		std::vector<t_listen>		getListen(void);
		std::vector<serverConfig*>	getServers(void);

		void	conf(char **);
		void	setup(); //? main funtion
		void	run();
};

#endif
