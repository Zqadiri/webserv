/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:27 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/16 15:34:55 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
// #include "server.hpp"

/*----- Exceptions-----*/

const char* Config::FileCorrupted::what() const throw() {
	return "File Corrupted";
}

/*---- Constructors & Destructor ------*/

Config::Config(){
}

Config::~Config(){
}

Config::Config(const Config &conf){
	*this = conf;
}

/*---- Operators -------*/

Config	&Config::operator=(const Config&){
	
	return *this;
}

/*---- Member Functions ----*/

configFile	Config::slitTokens(configFile con, std::string delim)
{
	configFile	tokens;

	// std::cout << str << std::endl;
	for (size_t i = 0; i < con.size(); i++)
	{
		std::string str(con[i]);
		size_t end = 0;
		size_t start = 0;
		str += delim[0];
		while (true)
		{
			end = str.find_first_of(delim, start);
			if (end == std::string::npos)
				break;
			// std::cout  << start << ":" << end << std::endl;
			std::string token = str.substr(start, end - start);
			// std::cout <<  " -> " << token << std::endl;
			tokens.push_back(token);
			start = str.find_first_not_of(delim, end);
			if (start == std::string::npos)
				break ;
		}
	}
	return tokens;
}

configFile	Config::readFile(const char *fileName){
	
	configFile con;
	configFile tokens;
	std::string str;

	std::string file(fileName);
	std::ifstream in(file.c_str());
	if (!in)
		throw Config::FileCorrupted();
	while (std::getline(in, str))
	{
		if (!str.length())
			continue;
		con.push_back(str);
	}
	// ! split to tokens
	return	(Config::slitTokens(con, " "));
}

void   		Config::parseFile(const char *fileName)
{
	configFile confFile;
	unsigned int	confSize;

	if (open(fileName, O_RDONLY) < 0)
		throw Config::FileCorrupted();
	confFile = Config::readFile(fileName);
	confSize = confFile.size();
	for (unsigned int i = 0; i < confSize; i++)
	{
		if (!confFile[i].compare("server"))
		{
			serverConfig server;
			++i;
			// if (confFile[i].compare("{"))
			// 	throw Config::FileNotWellFormated();
			// Config::parseServer();
			i = server.parseServer(confFile, i);
		}
	}
 
	
	// for(size_t i = 0; i < confFile.size(); i++)
	// 	std::cout << confFile[i] << std::endl;
}

// void		Config::parseServer(configFile con, unsigned int index)
// {
	
// }
