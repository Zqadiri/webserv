/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:36:44 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/17 18:40:09 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP_
#define SERVERCONFIG_HPP_

#include "../includes/webserv.hpp"

class serverConfig
{
	private:
		std::string					server_name;
		std::string					root;
		std::list<std::string>		allow_methods;
		std::vector<std::string>	locations;
		
	public:
		serverConfig();
		~serverConfig();
		serverConfig(const serverConfig &obj);
		serverConfig	&operator=(const serverConfig&);
		size_t serverName(const serverConfig&, configFile);

};

#endif
