/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 13:30:31 by nwakour           #+#    #+#             */
/*   Updated: 2022/06/01 10:42:07 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HPP__
#define __CGI_HPP__

#include "../../includes/webserv.hpp"
#include "../../Response/Response.hpp"
#include "../Request/request.hpp"
#include "../config.hpp"
#include "../utils/utils.hpp"

class Response;
class request;

class CGI
{
	private:
		std::map<std::string, std::string>	_env;
		std::string							_filePath;
		std::string							_scriptName;

	public:
		CGI(CGI const &);
		~CGI(void);
		CGI	&operator=(CGI const &src);
		CGI( request &request,  serverConfig &server);
		std::string		executeCgi(const std::string&, size_t, Response&);
		std::string		getTheCompletePath(const std::string&);
		std::string		addHeader(std::string, Response &);
};

#endif
