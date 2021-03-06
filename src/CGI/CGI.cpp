/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:08:22 by nwakour           #+#    #+#             */
/*   Updated: 2022/06/09 18:20:49 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#define GCI_BUFFERSIZE 1024

/*--------- Constructors & Destructor --------*/

CGI::CGI(request &request, serverConfig &config) : _scriptName("./cgi_bin/php-cgi"), _scripNamePy("./cgi_bin/py_cgi")
{
	std::map<std::string, std::string> _headers = request.getHeaders();
	if (_headers.find("Auth-Scheme") != _headers.end() && _headers["Auth-Scheme"] != "")
		this->_env["AUTH_TYPE"] = _headers["Authorization"];
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["CONTENT_LENGTH"] =  std::to_string(request.getBodyLength());
	this->_env["CONTENT_TYPE"] = _headers["Content-Type"];
	this->_env["REMOTE_ADDR"] = to_string(config.gethostPort().host);
	this->_env["QUERY_STRING"] = request.getQuery();
	this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
	this->_env["REMOTE_IDENT"] = _headers["Authorization"];
	this->_env["REMOTE_USER"] = _headers["Authorization"];
	if (_headers.find("Host") != _headers.end())
		this->_env["SERVER_NAME"] = _headers["Host"];
	else
		this->_env["SERVER_NAME"] = "serverName";
	this->_env["SERVER_PORT"] = to_string(config.gethostPort().port);
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Weebserv/1.0";
	// HTTP_*
	this->_env["HTTP_COOKIE"] = _headers["Cookie"];
	this->_env["HTTP_ACCEPT"] = _headers["Accept"];
	this->_env["HTTP_ACCEPT_CHARSET"] = _headers["Accept-Charset"];
	this->_env["HTTP_ACCEPT_ENCODING"] = _headers["Accept-Encoding"];
	this->_env["HTTP_ACCEPT_LANGUAGE"] = _headers["Accept-Language"];
	this->_env["HTTP_CONNECTION"] = _headers["Connection"];
}

CGI::CGI(CGI const &src){
	if (this != &src)
		this->_env = src._env;
}

CGI::~CGI(void){
}

CGI &CGI::operator=(CGI const &src){
	if (this != &src)
		this->_env = src._env;
	return *this;
}

/*--------- Member functions --------*/

char **mapToArray(std::map<std::string, std::string> _env)
{
	char **env;
	int j = 0;

	try{
		env = new char *[_env.size() + 1];
		for (std::map<std::string, std::string>::const_iterator i = _env.begin();
			 i != _env.end(); i++)
		{
			std::string line = i->first + "=" + i->second;
			env[j] = new char[line.size() + 1];
			env[j] = strcpy(env[j], line.c_str());
			j++;
		}
		env[j] = NULL;
	}
	catch (std::bad_alloc &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return env;
}

void deleteArray(char **env)
{
	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
}

/*
	main function for executing cgi
*/

std::string CGI::executeCgi(const std::string &_filePath, size_t socket_fd, Response &response, request &request)
{
	std::fstream fileOut;	
	std::string output;
	pid_t pid;
	int savedIn;
	int savedOut;
	char **env;
	std::string filename = "./tmp/body";
	filename += to_string(socket_fd);
	std::fstream _body;
	std::string myline;
	char **argv = new char *[3];
	argv[0] = new char[_scriptName.size() + 1];
	argv[0] = strcpy(argv[0], _scriptName.c_str());
	argv[1] = new char[_filePath.size() + 1];
	argv[1] = strcpy(argv[1], _filePath.c_str());
	argv[2] = NULL;

	this->_env["PATH_INFO"] = _filePath;
	this->_env["PATH_TRANSLATED"] = _filePath;
	env = mapToArray(this->_env);

	savedIn = dup(STDIN_FILENO);
	savedOut = dup(STDOUT_FILENO);

	response.get_file_change_get() = randomFileName();
	int fdIn = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
	int fdOut = open(response.get_file_change_get().c_str(),  O_RDWR | O_CREAT, 0666);

	lseek(fdIn, 0, SEEK_SET);
	pid = fork();
	if (pid == -1){
		std::cerr << "Fork Error" << std::endl;
		response.set_status_code(500);
	}
	else if (pid == 0){
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(_scriptName.c_str(), argv, env);
		response.set_status_code(500);
	}
	else
	{
		char buffer[GCI_BUFFERSIZE] = {0};
		int ret = 1;

		waitpid(pid, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		while (ret > 0)
		{
			memset(buffer, 0, GCI_BUFFERSIZE);
			ret = read(fdOut, buffer, GCI_BUFFERSIZE - 1);
			output += buffer;
		}
		dup2(savedIn, STDIN_FILENO);
		dup2(savedOut, STDOUT_FILENO);
	}
	fileOut.close();
	close(fdIn);
	close(fdOut);
	close(savedIn);
	close(savedOut);
	deleteArray(env);
	return addHeader(socket_fd, output, response, request);
}

std::string CGI::addHeader(int socket_fd, std::string output, Response &response, request &request)
{
	(void)socket_fd;
	time_t 			rawtime;
	time(&rawtime);
	std::string     filename(response.get_file_change_get());
	std::fstream  	myfile;

	response.get_str_uri() = filename;
	int fd = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC);
	response.get_header() += OK;
	response.get_header() += "Date: ";
	response.get_header() += std::string(ctime(&rawtime));

	response.get_header() += "Server: ";
	response.get_header() += "Myserver\r\n";

	response.get_header() += "Content-Type: ";
	response.get_header() += "text/html; charset=UTF-8";
	response.get_header() += "\r\n";

	if (request.getHeaders().find("Cookie") == request.getHeaders().end())
	{
		response.get_header() += "Set-Cookie: ";
		response.get_header() += "name=value; expires=Thu, 18 Dec 2013 12:00:00 GMT; path=/\r\n";
	}
	size_t end_headers = output.find_first_of("\n");
	int start = output.find("Content-type", 0);
	if (start != -1){
		int end = output.find("\n", start); 
		output = output.erase(start, end - start + 1);
	}
	response.set_body_length(response.get_body_length() + write (fd, output.substr(end_headers, output.length()).c_str(),  
							output.substr(end_headers, output.length()).length()));
	close(fd);
	response.get_header() += "Content-Length: ";
	response.get_header() += to_string(response.get_body_length());
	response.get_header() += "\r\n\r\n";
	std::cout << "get_str_uri() : "  << response.get_str_uri() << std::endl;	
	return output;
}