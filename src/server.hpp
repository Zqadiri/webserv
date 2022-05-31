/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tenshi <tenshi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 00:45:15 by nwakour           #+#    #+#             */
/*   Updated: 2022/06/01 00:13:37 by tenshi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "config.hpp"
#include "./Request/request.hpp"
#include "../Response/Response.hpp"

class	servers;
class 	request;
class server
{
	private:
		struct sockaddr_in		_addr;

		int						_fd;
		std::list<std::pair<int, request> >	_sockets;
		std::map<int, Response> _responses;
		std::vector<request*>	_requests;
		serverConfig* _config;

	public:
		server(void);
		server(t_listen&, serverConfig*);
		~server();
		server(const server&);
		server	&operator=(const server&);

		int		get_fd(void) const;
		
		int 	rec(int&, request&);
		int		sen(int&, request&, Response&);
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