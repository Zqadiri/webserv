/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:10 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/17 03:17:09 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "config.hpp"

class Server
{
	private:
		Config				config;
		std::vector<int>	_socket;
		int					_connection;
		struct sockaddr_in	_address;

	public:
		Server(void);
		Server(int, int, int, int, u_long);
		~Server();
		Server(const Server&);
		Server	&operator=(const Server&);

		struct sockaddr_in getAddress(void) const;
		int getConnection(void) const;
		int	getSocket(void) const;
		std::vector<t_listen>		getListen(void);
		void	conf(char **);

		int	connectNetwork (int, struct sockaddr_in);
		void	testConnection(int);

		std::vector<serverConfig*>	getServers(void);

};

#endif
