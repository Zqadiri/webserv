/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:08:22 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/15 14:48:13 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"


char *string_to_char(std::string str)
{
	char *cstr = new char[str.length() + 1];
	for(int i = 0; i < str.length(); ++i)
	{
		cstr[i] = str[i];
	}
	cstr[str.length()] = '\0';
	return cstr;
}

CGI::CGI(const request &request, const serverConfig &server)
{
	std::string tmp;
	_env = new char*[18];
	_env[17] = NULL;
	tmp = "AUTH_TYPE=";
	_env[0] = string_to_char(tmp);
	tmp = "CONTENT_LENGTH=";
	_env[1] = string_to_char(tmp);
	tmp = "CONTENT_TYPE=";
	_env[2] = string_to_char(tmp);
	tmp = "GATEWAY_INTERFACE=";
	_env[3] = string_to_char(tmp);
	tmp = "PATH_INFO=";
	_env[4] = string_to_char(tmp);
	tmp = "PATH_TRANSLATED=";
	_env[5] = string_to_char(tmp);
	tmp = "QUERY_STRING=";
	_env[6] = string_to_char(tmp);
	tmp = "REMOTE_ADDR=";
	_env[7] = string_to_char(tmp);
	tmp = "REMOTE_HOST=";
	_env[8] = string_to_char(tmp);
	tmp = "REMOTE_IDENT=";
	_env[9] = string_to_char(tmp);
	tmp = "REMOTE_USER=";
	_env[10] = string_to_char(tmp);
	tmp = "REQUEST_METHOD=";
	_env[11] = string_to_char(tmp);
	tmp = "SCRIPT_NAME=";
	_env[12] = string_to_char(tmp);
	tmp = "SERVER_NAME=";
	_env[13] = string_to_char(tmp);
	tmp = "SERVER_PORT=";
	_env[14] = string_to_char(tmp);
	tmp = "SERVER_PROTOCOL=";
	_env[15] = string_to_char(tmp);
	tmp = "SERVER_SOFTWARE=";
	_env[16] = string_to_char(tmp);
}
