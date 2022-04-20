/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:22:03 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/20 00:11:08 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

void        request::init_methods(){
	this->methods.push_back("GET");
	this->methods.push_back("DELETE");
	this->methods.push_back("POST");
}

/*------ Constructors ------*/

request::request() : _method(""), _requestURI(""), _version(""){
	init_methods();
	_retCode = 200;  // ? 200 OK -> Successful responses
}

request::~request()
{
}

/*------ Accessors ------*/

std::string								request::getMethod(){
	return      _method;
}
std::string								request::getRequestURI(){
	return      _requestURI;
}
std::string								request::getVersion(){
	return       _version;
}
std::map<std::string, std::string>		request::getHeaders(){
	return       _headers;
}

/*------ Member Functions ------*/

//! 400 Bad Request The server cannot or will not process
// the request due to something that is perceived to be a 
// client error ex: malformed request syntax

int			request::getFirstLine(const std::string &buff)
{
	std::string	line = buff.substr(0, buff.find_first_of('\n'));
	size_t	i, j;

	i = line.find_first_of(' ');
	if (i == std::string::npos)
		return -1;
	this->_method.assign(line, 0, i);
	if ((j = buff.find_first_not_of(' ', i)) == std::string::npos)
		return -1;
	if ((i = buff.find_first_of(' ', j)) == std::string::npos)
		return -1;
	this->_requestURI.assign(buff, j, i - j);
	j += _requestURI.size();
	if ((j = buff.find_first_not_of(' ', j)) == std::string::npos){
		std::cerr << "No HTTP version" << std::endl;
		return -1;
	}
	// std::cout <<  " --->  "  << buff.substr(j, buff.size()) << std::endl;
	if (buff[j] == 'H' && buff[j + 1] == 'T' && buff[j + 2] == 'T' &&
			buff[j + 3] == 'P' && buff[j + 4] == '/'){
		this->_version.assign(buff, j + 5, 3);
	}
	if (this->_version.compare("1.0") && this->_version.compare("1.1")){
		std::cerr << "BAD VERSION " << std::endl;
		return (-1);
	}
	std::cout << "path : " << this->_requestURI << std::endl;
	std::cout << "version :  "  << this->_version << std::endl;
	std::cout << "method :  "  << this->_method << std::endl;
	return j;
}

std::string				request::getNextLine(const std::string &buff, int &cursor){
	size_t i, end = 0;
	std::string ret;

	i = buff.find_first_of(' ', cursor);
	if (i == std::string::npos){
		end = 1;
		return "";
	} // !!
	ret = buff.substr(cursor, i);
	cursor = i;
	return ret;
}

int				request::startParsing(std::string buff){
	// std::cout << "<<< "<< buff << " >>>" <<std::endl;
	size_t  cursor = 0;
	std::string ret;
	cursor = this->getFirstLine(buff);
	// std::cout <<  " >>> "  << buff.substr(cursor, buff.size()) << std::endl;  // ! point to ' HTTP/1.1 .. '
	// while ((ret = getNextLine(buff, cursor)).compare(""))
	// {
	// 	std::cout << "ret >> " << ret << std::endl;
	// }
	return 1;
}

