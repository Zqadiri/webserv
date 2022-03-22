/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:27 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/18 11:50:14 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

const char* keys[] = {
	"server_names", 
	"root", 
	"listen", 
	"location", 
	"allow_methods",
	"index", 
	"error_page"
};

/*----- Exceptions-----*/

const char* Config::FileCorrupted::what() const throw() {
	return "File Corrupted";
}
const char* Config::FileNotWellFormated::what() const throw() {
	return "File Not Well Formated";
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

std::string		Config::removeSpace(std::string init)
{
	std::string ret;
	for (int i = 0; i < init.length(); ++i)
	{
		if (!isspace(init[i]))
			ret.push_back(init[i]);
	}
	return ret;
}

configFile		Config::slitTokens(configFile con, std::string delim)
{
	configFile	tokens;

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
			std::string token = removeSpace(str.substr(start, end - start));
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

void		Config::parseFile(const char *fileName)
{
	configFile confFile;
	unsigned int	confSize;

	if (open(fileName, O_RDONLY) < 0)
		throw Config::FileCorrupted();
	confFile = Config::readFile(fileName);
	//! check for unclosed curl
	// curlLevel(confFile);
	// for (size_t i = 0; i < confFile.size(); i++)
	// 	std::cout << "-+>  " << confFile[i] << std::endl;
	confSize = confFile.size();
	for (unsigned int i = 0; i < confSize; i++)
	{
		if (!confFile[i].compare("server"))
		{
			++i;
			if (!confFile[i].compare("{"))
				i = Config::parseServer(confFile, i);
		}
		// else
		// 	throw	Config::FileNotWellFormated();
	}
}

configFile::iterator	Config::curlLevel(configFile con)
{
	configFile::iterator it, ite;
	it = con.begin();
	ite  = con.end();
	size_t curlLvl = 0;
	while (it != ite)
	{
		if (*it == "server")
			break;
		if (*it == "{")
			curlLvl++;
		else if (*it == "}")
			curlLvl--;
		it++;
	}
	std::cout << "------> " << curlLvl << std::endl;
	if (curlLvl != 0)
		throw Config::FileNotWellFormated();
	return it;
}

typedef unsigned int (serverConfig::*Ptr)(serverConfig&, configFile, unsigned int&);

size_t		Config::parseServer(configFile con, unsigned int &index)
{
	serverConfig	server;
	bool isLocation = 0;
	configFile::iterator ite = con.end();
	configFile::iterator it = con.begin() + index;
	Ptr values[7] = {&serverConfig::serverName, &serverConfig::root,
	&serverConfig::listen, &serverConfig::location, &serverConfig::allowMethods,
	&serverConfig::index, &serverConfig::errorPages};
	while (true)
	{
		index++;
		for (size_t i = 0; i < 7; i++)
		{
			if (i == 3)
				isLocation = 1;
			if (index >= con.size())
				break;
			if (con[index] == keys[i])
				index = (server.*values[i])(server, con, index);
		}
		if (index >= con.size() || (con[index] == "}" && !isLocation))
			break;
	}
	std::cout << "---------" << std::endl;
	return index--;
}
