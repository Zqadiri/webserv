/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:36:44 by zqadiri           #+#    #+#             */
/*   Updated: 2022/06/04 22:02:54 by zqadiri          ###   ########.fr       */
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
	std::string				_pathCGI;
	bool					_autoindex;
	std::string				_uploadStore;
	t_redirect				_redirect;
};

//return STATUS_CODE PATH_TO_REDIRECT

class serverConfig
{
	public:
		// server data
		std::list<std::string>		_server_name;
		std::string					_root;
		std::string					_index;
		t_listen					_hostPort;
		bool						_autoindex;
		int							_limitBodySize;
		t_error_pages				_errorPages;
		std::list<std::string>		_allow_methods;
		std::vector<_location>		_locations; // vector of locations
		  
	public:
		serverConfig();
		serverConfig(std::string _root, std::string _index, std::string _host);
		~serverConfig();
		serverConfig(const serverConfig &obj);
		serverConfig	&operator=(const serverConfig&);

		const t_listen				 	&gethostPort(void) const;
		const std::list<std::string>	&getServerName(void) const;
		const std::vector<_location>	&getLocations(void) const;
		const t_error_pages 			&getErrorsPages(void) const;
		int								getlimitBodySize(void) const;
		bool							getAutoIndex(void) const;
		const std::list<std::string> 	&getAllowMethods(void) const;

		unsigned int 	serverName(serverConfig&, configFile, unsigned int&);
		unsigned int	parseLocation(serverConfig&, configFile, unsigned int&);
		unsigned int	limitBodySize(serverConfig &, configFile, unsigned int &);
		unsigned int	listen(serverConfig &, configFile, unsigned int&);
		unsigned int	root(serverConfig &, configFile, unsigned int&);
		unsigned int	index(serverConfig &, configFile, unsigned int&);
		unsigned int	errorPages(serverConfig &, configFile, unsigned int&);
		unsigned int	autoIndex(serverConfig &, configFile, unsigned int&);
		unsigned int	cgiPass(serverConfig &, configFile, unsigned int&);

		unsigned int	allowMethods(serverConfig &, configFile, unsigned int&);
		unsigned int	location(_location &, configFile, unsigned int &);
};

#endif
