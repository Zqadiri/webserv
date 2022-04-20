/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:22:03 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/20 03:04:04 by zqadiri          ###   ########.fr       */
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

int						request::getFirstLine(const std::string &buff)
{
	std::string	line = buff.substr(0, buff.find_first_of('\n'));
	size_t	i, j;

	i = line.find_first_of(' ');
	if (i == std::string::npos){
		this->_retCode = 400;
		std::cerr << "BAD REQUEST" << std::endl;
		return -1;
	}
	this->_method.assign(line, 0, i);
	if ((j = buff.find_first_not_of(' ', i)) == std::string::npos){
		this->_retCode = 400;
		std::cerr << "BAD REQUEST" << std::endl;
		return -1;
	}
	if ((i = buff.find_first_of(' ', j)) == std::string::npos){
		this->_retCode = 400;
		std::cerr << "BAD REQUEST" << std::endl;
		return -1;
	}
	this->_requestURI.assign(buff, j, i - j);
	j += _requestURI.size();
	if ((j = buff.find_first_not_of(' ', j)) == std::string::npos){
		this->_retCode = 400;
		std::cerr << "No HTTP version" << std::endl;
		return -1;
	}
	if (buff[j] == 'H' && buff[j + 1] == 'T' && buff[j + 2] == 'T' &&
			buff[j + 3] == 'P' && buff[j + 4] == '/'){
		this->_version.assign(buff, j + 5, 3);
	}
	if (this->_version.compare("1.0") && this->_version.compare("1.1")){
		//! The HTTP version used in the request is not supported by the server.
		this->_retCode = 505;
		std::cerr << "BAD VERSION" << std::endl;
		return (-1);
	}
	return j;
}

std::string				request::getNextLine(const std::string &buff, size_t &cursor)
{
	std::string		ret;
	size_t			i;

	if (cursor == std::string::npos)
		return "";
	i = buff.find_first_of('\n', cursor);
	ret = buff.substr(cursor, i - cursor);
	if (ret[ret.size() - 1] == '\r'){
		ret.resize(ret.size() - 1);
	}
	if (i != std::string::npos) //! skip new line
		i++;
	cursor = i;
	return ret;
}

std::string				request::getKey(const std::string &buff){
	std::string key;
	size_t i;

	i = buff.find_first_of(":", 0);
	key = buff.substr(0, i); //! check if the key is excepted
	return key;
}

std::string				request::getValue(const std::string &buff, size_t i){
	std::string value;
	size_t j;

	j = buff.find_first_of("\r", ++i);
	value = buff.substr(i, j);
	return value;
}

int						request::startParsing(std::string buff)
{
	size_t  cursor = 0;
	std::string ret, key, value;

	cursor = this->getFirstLine(buff);
	ret = getNextLine(buff, cursor); // ! skip ' HTTP/1.1 .. '
	while ((ret = getNextLine(buff, cursor)).compare("\r") && ret.compare("")){
		// std::cout << "<<< "<< ret << " >>>" <<std::endl;
		key = getKey(ret);
		value = getValue(ret, key.size());
		this->_headers.insert(std::make_pair(key, value));
	}
	this->_body = buff.substr(cursor, buff.size());
	print_req();
	return 1;
}

void	request::print_req(void)
{
	std::cout << "method :  "  << this->_method << std::endl;
	std::cout << "path : " << this->_requestURI << std::endl;
	std::cout << "version :  "  << this->_version << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
    it != this->_headers.end(); ++it){
   		std::cout << "{key} : " << it->first << std::endl;
		std::cout << "{value} : " << it->second  << std::endl;
	}
	std::cout << "body :  "  << this->_body << std::endl;
}
	