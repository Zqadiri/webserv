/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:45:15 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/19 02:06:10 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "config.hpp"
#include "./Request/request.hpp"

class	servers;
class 	request;

class server
{
	private:
		struct sockaddr_in		_addr;
		int						_port;
		int						_host;
		int						_fd;
		int						_socket;
		std::string				_rec;
		std::vector<request*>	_requests;

	public:
		server(void);
		server(t_listen&);
		~server();
		server(const server&);
		server	&operator=(const server&);

		int		get_fd(void) const;
		int		get_socket(void) const;
		void	set_socket(int);
		int		setup(void);
		int		acc(void);
		int		rec(void);
		void	print_rec(void);
};

#endif