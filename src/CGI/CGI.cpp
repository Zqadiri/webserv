/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:08:22 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/15 18:23:45 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

//! to_string c++11

CGI::CGI(const request &request, const serverConfig &server)
{
	std::map<std::string, std::string>	headers = request.getHeaders();
	// 'auth-scheme' token in the request Authorization header field
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		this->_env["AUTH_TYPE"] = headers["Auth-Scheme"];
	this->_env["CONTENT_TYPE"] = headers["Content-Type"];
	// Common Gateway Interface Version 1.1
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["PATH_INFO"] = request.getPath();
	// is derived by taking the PATH_INFO value
	this->_env["PATH_TRANSLATED"] = this->_env["PATH_INFO"];
	this->_env["QUERY_STRING"] = request.getQuery();
	//network address of the client sending the request to the server
	this->_env["REMOTE_ADDR"] = std::to_string(server.gethostPort().host);
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
	if (!server.getServerName().empty())
		this->_env["SERVER_NAME"] = server.getServerName().front(); //! list
	else
		this->_env["SERVER_NAME"] = this->_env["REMOTE_ADDR"];
	this->_env["SERVER_PORT"] = std::to_string(server.gethostPort().port);
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Webserv"; //?
	this->_env["CONTENT_LENGTH"] = std::to_string(request.getBodyLength());
	
}

