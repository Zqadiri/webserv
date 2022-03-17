/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:36:44 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/16 15:55:04 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP_
#define SERVERCONFIG_HPP_

#include "../includes/webserv.hpp"
#include "./config.hpp"

// typedef	std::vector<std::string>	configFile;

class serverConfig
{
	private:
		std::vector<configFile>		_servers;
		std::vector<std::string>	server_name;
	public:
		serverConfig();
		~serverConfig();
		serverConfig(const serverConfig &obj);
		serverConfig	&operator=(const serverConfig&);
		size_t			parseServer(std::vector<std::string> con,unsigned int &);
		std::vector<std::string>::iterator	curlLevel(std::vector<std::string> con,  unsigned int &);
};

#endif
