/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 13:30:31 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/15 22:12:18 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HPP__
#define __CGI_HPP__

#include "../../includes/webserv.hpp"
#include "../Request/request.hpp"
#include "../config.hpp"

class CGI
{

	private:
		std::map<std::string, std::string>	_env;

	public:
		CGI(const request &request, const serverConfig &server);
		std::string		executeCgi(const std::string&,  const serverConfig &, int);

};

#endif