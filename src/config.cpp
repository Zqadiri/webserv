/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:27 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/15 13:31:11 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "server.hpp"

/*----- Exceptions-----*/

const char* Config::FileCorrupted::what() const throw()
{
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

configFile	Config::readFile(const char *fileName){
	
	configFile con;
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
	return con;
}

void   		Config::parseFile(const char *fileName){
	
	configFile confFile;

	if (open(fileName, O_RDONLY) < 0)
		throw Config::FileCorrupted();
	confFile = Config::readFile(fileName);
	std::cout << confFile.size();
}

