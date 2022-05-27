/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:08:22 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/27 17:42:54 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

#define GCI_BUFFERSIZE 1024

/*
	This information is passed through QUERY_STRING header and by using QUERY_STRING
	environment variable it can be easily accessed in your CGI program. Only 1024 characters can be there in a request string as the GET method has the size limitation. Information
	can be passed by simply concatenating key-value pairs along with any URL.
*/

/*--------- Constructors & Destructor --------*/

CGI::CGI( request &request,  serverConfig &config)
{
	std::map<std::string, std::string>	_headers = request.getHeaders();

	if (_headers.find("Auth-Scheme") != _headers.end() && _headers["Auth-Scheme"] != "")
		this->_env["AUTH_TYPE"] = _headers["Authorization"];

	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_env["SCRIPT_NAME"] = request.getPath();
	this->_env["SCRIPT_FILENAME"] = "big_hello.php";

	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["CONTENT_LENGTH"] = to_string(request.getBodyLength());
	this->_env["CONTENT_TYPE"] = _headers["Content-Type"];
	this->_env["PATH_INFO"] = request.getPath();
	this->_env["PATH_TRANSLATED"] = request.getPath();
	
	this->_env["REMOTE_ADDR"] = to_string(config.gethostPort().host);
	this->_env["QUERY_STRING"] = request.getQuery();
	this->_env["REQUEST_URI"] = request.getPath() + request.getQuery();
	this->_env["REMOTE_IDENT"] = _headers["Authorization"];
	this->_env["REMOTE_USER"] = _headers["Authorization"];
	if (_headers.find("Host") != _headers.end())
		this->_env["SERVER_NAME"] = _headers["Host"];
	else
		this->_env["SERVER_NAME"] = this->_env["REMOTE_ADDR"];
	this->_env["SERVER_PORT"] = to_string(config.gethostPort().port);
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Weebserv/1.0";
}

CGI::CGI(CGI const &src) {
	if (this != &src) {
		this->_env = src._env;
	}
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

char **mapToArray(std::map<std::string, std::string>	_env)
{
	char **env;
	int j = 0;

	try{
		env = new char*[_env.size() + 1];
		for (std::map<std::string, std::string>::const_iterator i = _env.begin(); 
								i != _env.end(); i++)
		{
			std::string	line = i->first + "=" + i->second;
			env[j] = new char[line.size() + 1];
			env[j] = strcpy(env[j], line.c_str());
			std::cout << YELLOW <<  env[j]  << RESET << std::endl;
			j++;
		}
		env[j] = NULL;
	}
	catch (std::bad_alloc &e) {
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return env;
}

std::string	CGI::executeCgi(const std::string& scriptPath, size_t socket_fd, Response &response)
{
	(void) scriptPath;
	std::string 	scriptName = "./php-cgi";
	std::string 	output;
	pid_t			pid;
	int				savedIn;
	int				savedOut;
	char			**env;
	FILE			*fileIn = tmpfile();
	FILE			*fileOut = tmpfile();
	std::string 	filename = "/tmp/body";
	std::fstream	_body;

	env = mapToArray(this->_env);
	savedIn = dup(STDIN_FILENO);
	savedOut = dup(STDOUT_FILENO);

	// get the integer file descriptor associated with the stream pointed to by stream.
	long	fdIn = fileno(fileIn);
	long	fdOut = fileno(fileOut);

	filename += to_string(socket_fd);
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
	pid = fork();
	if (pid == -1){
		std::cerr << "Fork Error"<< std::endl;
		return ("");
	}
	else if (pid == 0)
	{
		std::cout << GREEN << "Script : " << scriptName << "  fd : " << socket_fd << RESET << std::endl;
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execle(scriptName.c_str(), "./www/big_hello.php", NULL, env_g);
		response._status_code = 500;
	}
	else
	{
		// get the output from the file to a string
		char	buffer[GCI_BUFFERSIZE] = {0};
		int		ret = 1;

		waitpid(pid, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		while (ret > 0)
		{
			memset(buffer, 0, GCI_BUFFERSIZE);
			ret = read(fdOut, buffer, GCI_BUFFERSIZE - 1);
			output += buffer;
			std::cout << GREEN << "output : " << output << RESET << std::endl;
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
	//!delete the array
 	return output;
}
