/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:22:03 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/14 14:52:43 by zqadiri          ###   ########.fr       */
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
	_port = 80;
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
}

request::~request(){
}

/*------ Accessors ------*/

const std::string							&request::getMethod() const { return	_method;}
const std::string							&request::getPath() const { return _path;}
const std::string							&request::getQuery() const { return _queryString;}
const std::string							&request::getVersion() const { return  _version;}
const std::string							&request::getRequestURI() const { return	_requestURI;}
const std::string							&request::getHost() const { return _host; }
const int									&request::getPort() const { return _port; }
const std::map<std::string, std::string>	&request::getHeaders() const { return  _headers;}
int											request::getRetCode() const { return _retCode; }

void				request::setCode(int code){
	this->_retCode = code;
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

int			badRequest(request& req){
	req.setCode(400);
	std::cerr << "BAD REQUEST" << std::endl;
	return -1;
}

int					request::checkMethod()
{
	for (size_t i = 0; i < this->possibleMethods.size(); i++)
		if (this->possibleMethods[i] == this->_method)
			return this->_retCode;
	std::cerr << YELLOW << "Invalid Method" << RESET << std::endl;
	this->_retCode = 400;
	return 0;
}

int					request::getFirstLine(const std::string &buff, request& req)
{
	std::string	line = buff.substr(0, buff.find_first_of('\n'));
	size_t	i, j;

	i = line.find_first_of(' ');
	if (i == std::string::npos)
		return badRequest(req);
	req._method.assign(line, 0, i);
	if (!req.checkMethod())
		return -1;
	if ((j = buff.find_first_not_of(' ', i)) == std::string::npos)
		return badRequest(req);
	if ((i = buff.find_first_of(' ', j)) == std::string::npos)
		return badRequest(req);
	req._path.assign(buff, j, i - j);
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
	_status = PRE_HEADERS; 
	return j;
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
	if (i != std::string::npos)
		i++;
	cursor = i;
	return ret;
}

void				request::getQuery(){
	size_t		i =  this->_path.find_first_of('?');

	if (i != std::string::npos){
		this->_queryString = removeSpace(this->_path.substr( i, std::string::npos));
		this->_path = this->_path.substr(0, i);
	}
}

int					request::ParseHeaders(std::string buff,  request& req)
{
	std::string ret, key, value;
	size_t cursor = 0;

	cursor = getFirstLine(buff, req);
	ret = getNextLine(buff, cursor);
	while ((ret = getNextLine(buff, cursor)).compare("\r") && ret.compare(""))
	{
		key = removeSpace(getKey(ret));
		if (!key.compare("Host")){
			int begin = ret.find_first_of(":");
			begin++;
			std::string str = ret.substr(begin, ret.length());
			Host(str, req);
			continue;
		}
		value = removeSpace(getValue(ret, key.size()));
		req._headers[key] = value;
	}
	req.getQuery();
	_status = PRE_BODY;
	// print_req(req);
	return 1;
}

int toHex(std::string str){
	std::stringstream ss;
	size_t hex;

	ss << std::hex << str;
	ss >> hex;
	return hex;
}

int					request::parseRquest(std::string buff,  request& req, int socket_fd){
	std::fstream							_body;
	std::string delim("\r\n\r\n");
	std::string filename = "/tmp/body";
	size_t bodyCursor = buff.find(delim);
	
	filename += std::to_string(socket_fd);
	if (bodyCursor == std::string::npos && _status == START_LINE){
		req._tmp += buff;
	}
	else if (_status == START_LINE){
		req._tmp.append(buff.substr(0, bodyCursor + delim.length()));
		_status = HEADERS;
	}
	if (_status == HEADERS)
		ParseHeaders(req._tmp, req);
	if (_status == PRE_BODY){
		if (_headers["Transfer-Encoding"].compare("chunked")){
			_body.open (filename, std::fstream::in | std::fstream::out | std::fstream::app);
			if(_body.is_open()){
				// size_t end = _tmp.find("\r\n");
				// _tmp.erase(0);
				_body << _tmp.c_str();
			}
			else{
				_retCode = 500;
				return -1;				
			}
			_status = COMPLETE;
		}
		else if (!_headers["Transfer-Encoding"].compare("chunked") && _headers["Content-Length"].compare("")){
			req._tmp.clear();
			req._tmp.append(buff.substr(bodyCursor + delim.length(), buff.length()));
			buff.clear();
			_status = BODY;
			_chunkStatus = SIZE_LINE;
		}
	}
	if (_status == BODY){
		this->_tmp += buff;
		parseChunkedRequest(filename);
	}
	if (_status == COMPLETE)
		return 0;
	return 1;
}

int request::parseChunkedRequest(std::string filename) // !1 not comp
{
	size_t end;
	std::fstream _body;

	while ((end = _tmp.find("\r\n")) != std::string::npos)
	{
		if (_chunkStatus == SIZE_LINE)
		{
			std::string hex = _tmp.substr(0, end);
			_chunkSize = toHex(hex);
			_tmp.erase(0, end + 2);
			_chunkStatus = CHUNK;
		}
		else if (_chunkStatus == CHUNK)
		{
			if (_chunkSize == 0)
				return 1;
			_body.open (filename, std::fstream::in | std::fstream::out | std::fstream::app);
			if(_body.is_open())
				_body << _tmp.substr(0, end);
			else
				puts("error open");
			_tmp.erase(0, end + 2);
			_chunkSize = 0;
			_chunkStatus = SIZE_LINE;
		}
	}
	return 0;
}

void				request::print_req(request& req)
{
	std::cout << "method :  ["  << req._method << "]" << std::endl;
	std::cout << "path : [" << req._requestURI << "]" << std::endl;
	std::cout << "version :  ["  << req._version << "]" << std::endl;
	std::cout << "Host :  ["  << req._host<< "]"  << std::endl;	
	std::cout << "Port :  ["  << req._port << "]" << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = req._headers.begin();
	it != req._headers.end(); ++it){
		std::cout  << it->first << std::endl;
		std::cout << " : [" << it->second << "]" << std::endl;
	}
}
