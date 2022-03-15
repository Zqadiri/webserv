/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:10 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/15 13:39:23 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "../includes/webserv.hpp"
#include "config.hpp"

// class Config;

class Server
{
	private:
    	Config  config;		
	public:
		Server();
		~Server();
		Server(const Server&);
		Server	&operator=(const Server&);
		void	conf(char **);
};

#endif
