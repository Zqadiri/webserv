/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 12:15:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/06 16:31:31 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "../includes/webserv.hpp"

class Socket
{
	private:

		std::vector<int>	_socket;
		int					_connection;
		struct sockaddr_in	_address;
		int					_backlog;
		
	public:
		Socket();
		~Socket();
		Socket(int, int, int, int, u_long);
		Socket(const Socket &obj);
		Socket	&operator=(const Socket&);

			// Accessors 
		struct sockaddr_in getAddress(void) const;
		int getConnection(void) const;
		int	getSocket(void) const;


		virtual int	connectNetwork (int, struct sockaddr_in) = 0;
		void	testConnection(int);
};

#endif
