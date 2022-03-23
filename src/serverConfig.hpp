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

struct _location
{
	std::string				_path;
	std::list<std::string>	_allow_methods;
	std::string				_root;
	std::string				_index;
	int						_limitBodySize;
	bool					_alias;
	std::vector<_location>	_locations; //?
};

class serverConfig
{
	public: //! add getters 
		std::list<std::string>		_server_name;
		std::string					_root;
		std::string					_index;
		std::string					_host;
		unsigned int				_port;
		std::list<std::string>		_error_pages;
		std::list<std::string>		_allow_methods;
		std::vector<_location>		_locations;
		
	public:
		serverConfig();
		serverConfig(std::string _root, std::string _index, std::string _host);
		~serverConfig();
		serverConfig(const serverConfig &obj);
		serverConfig	&operator=(const serverConfig&);
		unsigned int 	serverName(serverConfig&, configFile, unsigned int&);
		unsigned int	location(serverConfig&, configFile, unsigned int&);
		unsigned int	listen(serverConfig &, configFile, unsigned int&);
		unsigned int	root(serverConfig &, configFile, unsigned int&);
		unsigned int	index(serverConfig &, configFile, unsigned int&);
		unsigned int	errorPages(serverConfig &, configFile, unsigned int&);
		unsigned int	allowMethods(serverConfig &, configFile, unsigned int&);

};

#endif
