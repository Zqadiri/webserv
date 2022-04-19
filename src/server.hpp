/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:45:15 by nwakour           #+#    #+#             */
/*   Updated: 2022/04/19 05:42:47 by tenshi           ###   ########.fr       */
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
		std::list<std::pair<int, request> >	_sockets;
		// std::string				_rec;
		// std::vector<request*>	_requests;

	public:
		server(void);
		server(t_listen&);
		~server();
		server(const server&);
		server	&operator=(const server&);

		int		get_fd(void) const;
		int 	rec(int&, request&);
		void 	handle_sockets(fd_set&);
		void 	add_socket(fd_set&, int&);
		int		setup(void);
		int		acc(void);
		bool 	is_sockets_empty(void) const;
		void	print_rec(void);
};

#endif