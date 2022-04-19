/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:22:03 by zqadiri           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/04/19 16:11:41 by zqadiri          ###   ########.fr       */
=======
/*   Updated: 2022/04/19 22:00:57 by nwakour          ###   ########.fr       */
>>>>>>> 22b4002232453a19e39985a130f988ef754430af
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

<<<<<<< HEAD
int			request::getFirstLine(const std::string &buff)
{
	std::string	line = buff.substr(0, buff.find_first_of('\n'));
	size_t	i;
	size_t	j;

	i = line.find_first_of(' ');
	if (i == std::string::npos)
		return -1;
	this->_method.assign(line, 0, i);

	if ((j = buff.find_first_not_of(' ', i)) == std::string::npos)
		return -1;
	if ((i = buff.find_first_of(' ', j)) == std::string::npos)
		return -1;
	this->_requestURI.assign(buff, j, i - j);
	j += 2;
	// std::cout <<  " >>> "  << buff.substr(cur, buff.size()) << std::endl;
	if ((j = buff.find_first_not_of(' ', j)) == std::string::npos){
		std::cerr << "No HTTP version" << std::endl;
		return -1;
	}
	if (buff[j] == 'H' && buff[j + 1] == 'T' && buff[j + 2] == 'T' &&
			buff[j + 3] == 'P' && buff[j + 4] == '/')
		this->_version.assign(buff, j + 5, 3);
	if (this->_version != "1.0" && this->_version != "1.1"){
		std::cerr << "BAD VERSION " << std::endl;
		return (-1);
	}
	std::cout << "method :  "  << this->_method << std::endl;
	std::cout << "path : " << _requestURI << std::endl;
	std::cout << "version :  "  << this->_version << std::endl;
	return j;
=======
int			request::getFirstLine(std::string buff){
    std::string delim("\n");
    int end = buff.find_first_of(delim, 0);
    (void)end;
    return 1;
>>>>>>> 22b4002232453a19e39985a130f988ef754430af
}

int				request::startParsing(std::string buff){
	size_t  cursor = 0;
	cursor = this->getFirstLine(buff);
	// std::cout <<  " >>> "  << buff.substr(cursor, buff.size()) << std::endl;  // ! point to ' HTTP/1.1 .. '

	return 1;
}

