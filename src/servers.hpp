/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:10 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/19 02:33:22 by nwakour          ###   ########.fr       */
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
		Config					config; //? servers data
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
