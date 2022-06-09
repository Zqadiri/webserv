/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 16:11:17 by zqadiri           #+#    #+#             */
/*   Updated: 2022/06/09 17:42:50 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

int			badRequest(request& req){
	req.setCode(400);
	return -1;
}

int toHex(std::string str){
	std::stringstream ss;
	size_t hex;

	ss << std::hex << str;
	ss >> hex;
	return hex;
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
	req._path.assign(line, j, i - j);
	j += i - j;
	if ((j = buff.find_first_not_of(' ', j)) == std::string::npos){
		req._retCode = 400;
		std::cerr << "No HTTP version" << std::endl;
		return -1;
	}
	if (buff[j] == 'H' && buff[j + 1] == 'T' && buff[j + 2] == 'T' &&
			buff[j + 3] == 'P' && buff[j + 4] == '/'){
		req._version.assign(line, j + 5, 3);
	}
	if (req._version.compare("1.0") && req._version.compare("1.1")){
		req._retCode = 505;
		std::cerr << "BAD VERSION" << std::endl;
		return (-1);
	}
	_status = PRE_HEADERS; 
	return j;
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
	req.getQueryString();
	if (_headers["Authorization"].compare(""))
		parseAuthorization(req);
	if (_headers["Transfer-Encoding"].compare("") && _headers["Transfer-Encoding"].compare("chunked")){
		this->_retCode = 501; 
		return -1;
	}
	if (!_method.compare("GET") || !_method.compare("DELETE")){
		_status = COMPLETE;
		return 1;
	}
	if (!_headers["Content-Length"].compare("")){
		this->_retCode = 411; 
		return -1;
	}
	_status = PRE_BODY;
	return 1;
}

int					request::InternalServerError(){
	std::cerr << YELLOW << "InternalServerError" << RESET << std::endl;
	_retCode = 500;
	return 0;	
}

int					request::parseRquest(std::string buff,  request& req, int socket_fd)
{
	// std::fstream _body;
	std::string delim("\r\n\r\n");
	std::string filename = "./tmp/body";
	size_t bodyCursor = buff.find(delim);
	std::cout << RED << buff << RESET << std::endl;
	reset_timer();
	filename += to_string(socket_fd);
	if (!_body.is_open())
		_body.open (_name, std::fstream::in | std::fstream::out | std::fstream::trunc);
	if (bodyCursor == std::string::npos && _status == START_LINE)
		req._tmp += buff;
	else if (_status == START_LINE){
		req._tmp.append(buff.substr(0, bodyCursor + delim.length()));
		_status = HEADERS;
	}
	if (_status == HEADERS)
	{
		if (ParseHeaders(req._tmp, req) < 0) 
			return 0;
	}
	if (_status == PRE_BODY){
		req._tmp.clear();
		req._tmp.append(buff.substr(bodyCursor + delim.length(), buff.length()));
		buff.clear();
		if (_headers["Transfer-Encoding"].compare("chunked")){
			std::cout << GREEN  <<"Chunked"<< RESET << std::endl;
			_status = BODY;
		}
		else if (!_headers["Transfer-Encoding"].compare("chunked")){
			std::cout << GREEN  <<"Unchunked" << RESET << std::endl;
			_status = CHUNKS;
			_chunkStatus = SIZE_LINE;
		}
	}
	if (_status == BODY){
		// this->_tmp += buff;
		this->_tmp.append(buff);
		parseUnchunkedRequest(filename);
	}
	if (_status == CHUNKS){
		this->_tmp += buff;
		parseChunkedRequest(filename);
	}
	if (_status == COMPLETE){
		std::cout << "COMPLETE "  << _bodyLength << std::endl;
		_body.close();
		return 0;
	}
	std::cout << RED <<_status<< RESET <<std::endl;
	return 1;
}

int request::parseUnchunkedRequest(std::string filename)
{
	(void)filename;
	if(_body.is_open()){
		_bodyLength += _tmp.length();
		_body << _tmp;
	}
	else
		return InternalServerError();
	_tmp.clear();
	if (_headers["Content-Length"].compare("") && _bodyLength >= stoi(_headers["Content-Length"])){
		_body << "\n";
		_body.close();
		_status = COMPLETE;
	}
	return 0;
}

int request::parseChunkedRequest(std::string filename)
{
	size_t end;
	(void)filename;

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
			if (_chunkSize == 0){
				_body << "\n";
				_body.close();
				_status = COMPLETE;
				return 1;
			}
			if(_body.is_open()){
				_bodyLength += _tmp.length();
				_body << _tmp.substr(0, end);
			}
			else
				return InternalServerError();
			_tmp.erase(0, end + 2);
			_chunkSize = 0;
			_chunkStatus = SIZE_LINE;
		}
	}
	return 0;
}

