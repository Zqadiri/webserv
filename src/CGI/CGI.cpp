/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:08:22 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/16 11:59:31 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#define GCI_BUFFERSIZE 1024
//! to_string c++11

/*--------- Constructors & Destructor --------*/

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

CGI::CGI(CGI const &src) {
	if (this != &src) {
		this->_env = src._env;
	}
	return ;
}

CGI::~CGI(void) {
	return ;
}

CGI	&CGI::operator=(CGI const &src) {
	if (this != &src) {
		this->_env = src._env;
	}
	return *this;
}

/*--------- Member functions --------*/

// https://en.cppreference.com/w/cpp/io/c/tmpfile

std::string		CGI::executeCgi(const std::string& scriptName,  const serverConfig &server, int socket_fd)
{
	(void)server;
	std::string output;
	pid_t		pid;
	int			savedIn;
	int			savedOut;
	int			j;
	char		**env;
	FILE		*fileIn = tmpfile();
	FILE		*fileOut = tmpfile();
	std::string filename = "/tmp/body";
	std::fstream	_body;

	j = 0;
	try{
		env = new char*[this->_env.size() + 1];
		for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); 
								i != this->_env.end(); i++)
		{
			std::string	line = i->first + "=" + i->second;
			env[j] = new char[line.size() + 1];
			env[j] = strcpy(env[j], line.c_str());
			j++;
		}
		env[j] = NULL;
	}
	catch (std::bad_alloc &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	savedIn = dup(STDIN_FILENO);
	savedOut = dup(STDOUT_FILENO);

	// get the integer file descriptor associated with the stream pointed to by stream.
	long	fdIn = fileno(fileIn);
	long	fdOut = fileno(fileOut);

	filename += std::to_string(socket_fd);
	_body.open(filename, std::fstream::in);
	if(!_body) {
		std::cerr << "Error" << std::endl;
		exit(1);
	}
	std::string myline; 
	while (_body)
	{
		std::getline (_body, myline);
		write(fdIn,myline.c_str(), myline.length());
	}
	//system call that is used to change the location of the read/write pointer of a file descriptor
	lseek(fdIn, 0, SEEK_SET);
	
	pid = fork();
	if (pid == -1){
		std::cerr << "Fork Error"<< std::endl;
		return ("");
	}
	else if (pid == 0)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(scriptName.c_str(), NULL, env);
		std::cerr << "Execve Error"  << std::endl;
	}
	else
	{
		// get the outout from the file to a the return string
		char	buffer[GCI_BUFFERSIZE] = {0};
		int		ret = 1;

		waitpid(-1, NULL, 0);
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
	close(fdIn);
	close(fdOut);
	fclose(fileIn);
	fclose(fileOut);
	close(savedIn);
	close(savedOut);
	return output;
}
