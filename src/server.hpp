/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:45:15 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/15 16:41:26 by nwakour          ###   ########.fr       */
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

		int						_fd;
		std::list<std::pair<int, request> >	_sockets;
		std::vector<request*>	_requests;

	public:
		server(void);
		server(t_listen&);
		~server();
		server(const server&);
		server	&operator=(const server&);

		int		get_fd(void) const;
		
		int 	rec(int&, request&);
		int		sen(int&, request&);
		void 	handle_sockets(fd_set&, fd_set&,fd_set&, fd_set&);
		int 	add_socket(fd_set&,fd_set&, int&);
		int		setup(void);
		int		acc(void);
		bool 	is_sockets_empty(void) const;
		void	print_rec(void);
		void 	check_timeout(fd_set&, const std::time_t&);

		//! mine
		std::list<std::pair<int, request> >		getRequest(void);
		
};

#endif