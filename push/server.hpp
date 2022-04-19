/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:45:15 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/19 02:39:40 by nwakour          ###   ########.fr       */
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
		std::list<int>	_sockets;
		std::string			_rec;

	public:
		server(void);
		server(t_listen&);
		~server();
		server(const server&);
		server	&operator=(const server&);

		int get_fd(void) const;
		// int get_sockets(void) const;
		void add_socket(int&);
		int setup(void);
		int acc(void);
		int rec(int&);
		int handle_sockets(fd_set&);
		void print_rec(void);
};

#endif