/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:10 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/17 23:57:41 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "config.hpp"

class Server // add s
{
	private:
		Config				config; //? servers data
		std::vector<int>	_socket; //? sockets fds

	public:
		Server(void);
		Server(int, int, int, int, u_long);
		~Server();
		Server(const Server&);
		Server	&operator=(const Server&);

		std::vector<t_listen>		getListen(void);
		std::vector<serverConfig*>	getServers(void);

		void	conf(char **);
		void	setup(); //? main funtion
		
};

#endif
