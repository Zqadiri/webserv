/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:22:03 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/08 14:54:38 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

/*---- Init Static Members -----*/

std::vector<std::string>	request::possibleMethods = request::init_methods();

std::vector<std::string>		request::init_methods()
{
	std::vector<std::string>	methods;
	methods.push_back("GET");
	methods.push_back("DELETE");
	methods.push_back("POST");
	methods.push_back("HEAD");
	methods.push_back("PUT");
	methods.push_back("CONNECT");
	methods.push_back("OPTIONS");
	methods.push_back("TRACE");	
	return methods;
}

/*------ Constructors ------*/

request::request() : _method(""), _requestURI(""), _version(""), _host(""){
	_retCode = 200;  // ? 200 OK -> Successful responses
	_port = -1;
}

request::~request(){
}

/*------ Accessors ------*/

std::string							request::getMethod() const { return	_method;}
std::string							request::getRequestTarget() const { return _requestTarget;}
std::string							request::getVersion() const { return  _version;}
std::string							request::getRequestURI() const { return	_requestURI;}
std::string							request::getHost() const { return _host; }
int									request::getPort() const { return _port; }
std::map<std::string, std::string>	request::getHeaders() const { return  _headers;}
int									request::getRetCode() const { return _retCode; }

void		request::setCode(int code){
	this->_retCode = code;
}

void				request::setBody(const std::string& str){
	this->_body.assign(str);
	// ! check format
}

/*------ Member Functions ------*/

std::string				request::getKey(const std::string &buff){
	std::string key;
	size_t i;

	i = buff.find_first_of(":", 0);
	key = buff.substr(0, i);
	return key;
}

std::string			request::getValue(const std::string &buff, size_t i){
	std::string value;
	size_t j;

	j = buff.find_first_of("\r", ++i);
	value = buff.substr(i, j);
	return value;
}

int			badRequest(request& req){
	req.setCode(400);
	std::cerr << "BAD REQUEST" << std::endl;
	return -1;
}

int					request::getFirstLine(const std::string &buff, request& req)
{
	std::string	line = buff.substr(0, buff.find_first_of('\n'));
	size_t	i, j;

	i = line.find_first_of(' ');
	if (i == std::string::npos)
		return badRequest(req);
	req._method.assign(line, 0, i);
	if ((j = buff.find_first_not_of(' ', i)) == std::string::npos)
		return badRequest(req);
	if ((i = buff.find_first_of(' ', j)) == std::string::npos)
		return badRequest(req);
	req._requestTarget.assign(buff, j, i - j);
	j += i - j;
	if ((j = buff.find_first_not_of(' ', j)) == std::string::npos){
		req._retCode = 400;
		std::cerr << "No HTTP version" << std::endl;
		return -1;
	}
	if (buff[j] == 'H' && buff[j + 1] == 'T' && buff[j + 2] == 'T' &&
			buff[j + 3] == 'P' && buff[j + 4] == '/'){
		req._version.assign(buff, j + 5, 3);
	}
	if (req._version.compare("1.0") && req._version.compare("1.1")){
		req._retCode = 505;
		std::cerr << "BAD VERSION" << std::endl;
		return (-1);
	}
	// req.checkMethod();
	return j;
}

//! 405 Method Not Allowed

void				request::Host(const std::string &str, request & req){
	int end = str.find_first_of(":");
	if (end == std::string::npos){
		req._host = str.substr(0, str.length());
		return ;
	}
	req._host = str.substr(0, end);
	end++;
	req._port = stoi(str.substr(end, str.length()));
	return;
}

std::string			request::getNextLine(const std::string &buff, size_t &cursor)
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

int 				request::checkData(std::string buff,  request& req, size_t cur)
{
	std::string str = buff.substr(cur, buff.length());
	std::cout << " **>" << buff << "<**"  << std::endl;
	// if (req._headers["WWW-Authenticate"].compare("") != 0)
	// 	authenticate();
	// req.setBody();
	return cur;
}

int					request::startParsing(std::string buff,  request& req)
{
	size_t  cursor = 0;
	std::string ret, key, value;

	cursor = this->getFirstLine(buff, req);
	ret = getNextLine(buff, cursor);
	while ((ret = getNextLine(buff, cursor)).compare("\r") && ret.compare("")){
		key = getKey(ret);
		if (!key.compare("Host")){
			int begin = ret.find_first_of(":");
			begin++;
			std::string str = ret.substr(begin, ret.length());
			Host(str, req);
			continue;
		}
		value = getValue(ret, key.size());
		req._headers.insert(std::make_pair(key, value));
	}
	this->checkData(buff, req, cursor);
	// print_req(req);
	return 1;
}

void				request::print_req(request& req)
{
	std::cout << "method :  "  << req._method << std::endl;
	std::cout << "path : " << req._requestURI << std::endl;
	std::cout << "version :  "  << req._version << std::endl;
	std::cout << "Host :  "  << req._host << std::endl;	
	if (req._port != -1)
		std::cout << "Host :  "  << req._port << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = req._headers.begin();
	it != req._headers.end(); ++it){
		std::cout  << it->first << std::endl;
		std::cout << " : " << it->second  << std::endl;
	}
	std::cout << "body :  "  << req._body << std::endl;
}
