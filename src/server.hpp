/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:45:15 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/18 16:44:11 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "config.hpp"

class servers;

class server
{
	private:
		struct sockaddr_in	_addr;
		int					_port;
		int					_host;
		int					_fd;

	public:
		server(void);
		server(t_listen&);
		~server();
		server(const server&);
		server	&operator=(const server&);

		int get_fd(void) const;
		int setup(void);
		int acc(void);
};

#endif