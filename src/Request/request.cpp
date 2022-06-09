/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:22:03 by zqadiri           #+#    #+#             */
/*   Updated: 2022/06/09 17:11:33 by zqadiri          ###   ########.fr       */
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

request::request(int socket_fd) : _method(""), _requestURI(""), _version(""), _host(""), _current_time(std::time(NULL)){
	
	_name = "./tmp/body";
	_name += to_string(socket_fd);
	_retCode = 200;  // ? 200 OK -> Successful responses
	_port = 80;
	_bodyLength = 0;
	_status = START_LINE;
	this->_headers["Accept"] = ""; 
	this->_headers["Accept-Charsets"] = "";
	this->_headers["Accept-Language"] = "";
	this->_headers["Referer"] = "";
	this->_headers["Allow"] = "";
	this->_headers["Auth-Scheme"] = "";
	this->_headers["Authorization"] = "";
	this->_headers["Connection"] = "";
	this->_headers["Content-Language"] = "";
	this->_headers["Content-Length"] = "";
	this->_headers["Content-Location"] = "";
	this->_headers["Content-Type"] = "";
	this->_headers["Location"] = "";
	this->_headers["Host"] = "";
	this->_headers["Transfer-Encoding"] = "";
	this->_headers["User-Agent"] = "";
	this->_headers["Www-Authenticate"] = "";
	this->_headers["User-Agent"] = "";
	this->_headers["Content-Disposition"] = "";
	// _body.open (_name, std::fstream::in | std::fstream::out | std::fstream::trunc);
	// _body.close();
}

request::request(request const &cp)
{
	*this = cp;
}

request &request::operator=(request const &cp)
{
	
	_headers = cp._headers;
	_method = cp._method;
	_requestURI = cp._requestURI;
	_version = cp._version;
	_host = cp._host;
	_current_time = cp._current_time;
	_retCode = cp._retCode;
	_port = cp._port;
	_bodyLength = cp._bodyLength;
	_status = cp._status;
	_name = cp._name;
	// _body.open (_name, std::fstream::in | std::fstream::out | std::fstream::trunc);
	return *this;
}
request::~request(){
} 

/*------ Accessors ------*/

std::string									&request::getContentFromMap(std::string key){
	return this->_headers[key];
}
const std::string							&request::getMethod() const { return	_method;}
const std::string							&request::getPath() const { return _path;}
const std::string							&request::getQuery() const { return _queryString;}
const std::string							&request::getVersion() const { return  _version;}
const std::string							&request::getRequestURI() const { return	_requestURI;}
const std::string							&request::getHost() const { return _host; }
const int									&request::getPort() const { return _port; }
const std::map<std::string, std::string>	&request::getHeaders() const { return  _headers;}
int											request::getRetCode() const { return _retCode; }
const time_t								&request::getTime() const { return _current_time; }
int											request::getBodyLength() const{ return	_bodyLength; }
void										request::setCode(int code){
	this->_retCode = code;
}
void request::reset_timer(){
	_current_time = std::time(NULL);
}

/*------ Member Functions ------*/

std::string			request::getKey(const std::string &buff){
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

int					request::checkMethod()
{
	for (size_t i = 0; i < this->possibleMethods.size(); i++)
		if (this->possibleMethods[i] == this->_method)
			return this->_retCode;
	this->_retCode = 400;
	return 0;
}

void				request::Host(const std::string &str, request & req){
	size_t end = str.find_first_of(":");
	if (end == std::string::npos){
		req._host = removeSpace(str.substr(0, str.length()));
		return ;
	}
	req._host = removeSpace(str.substr(0, end));
	end++;
	req._port = stoi(str.substr(end, str.length()));
	return;
}

void				request::getQueryString(){
	size_t		i =  this->_path.find_first_of('?');

	if (i != std::string::npos){
		int j = i + 1;
		this->_queryString = removeSpace(this->_path.substr( j, std::string::npos));
		this->_requestURI = this->_path.substr(0, i);
	}
	else
		this->_requestURI = this->_path;
}

void				request::parseAuthorization(request& req){
	(void)req;
	size_t end = _headers["Authorization"].find_first_of(' ');
	_headers["Auth-Scheme"] = _headers["Authorization"].substr(0, end);
}

void				request::closeStream(){
	if (_body.is_open())
		_body.close();
}

/*
	case1: unchunked + no content-length
	case2: unchunked + content-length
	case3: chuncked
*/

void				request::print_req(request& req){
	std::cout << "method :  ["  << req._method << "]" << std::endl;
	std::cout << "path : [" << req._path << "]" << std::endl;
	std::cout << "requestURI : [" << req._requestURI << "]" << std::endl;
	std::cout << "version :  ["  << req._version << "]" << std::endl;
	std::cout << "Host :  ["  << req._host<< "]"  << std::endl;	
	std::cout << "Port :  ["  << req._port << "]" << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = req._headers.begin();
	it != req._headers.end(); ++it){
		std::cout  << it->first << std::endl;
		std::cout << " : [" << it->second << "]" << std::endl;
	}
}
