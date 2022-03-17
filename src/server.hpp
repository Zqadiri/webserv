/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:10 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/17 16:04:22 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/webserv.hpp"
#include "config.hpp"

class Server
{
	Config  config;	
	// std::vector<serverConfig> servers
	public:
		Server();
		~Server();
		Server(const Server&);
		Server	&operator=(const Server&);
		void	conf(char **);
};

#endif
