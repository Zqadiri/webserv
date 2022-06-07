/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:27 by zqadiri           #+#    #+#             */
/*   Updated: 2022/06/07 19:14:22 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include <memory>

typedef unsigned int (serverConfig::*Ptr)(serverConfig&, configFile, unsigned int&);

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
	for (size_t i = 0; i < servers.size(); i++)
		delete servers[i];
}

Config::Config(const Config &conf){
	*this = conf;
}

/*---- Accessors -------*/

const std::vector<serverConfig*>	&Config::getServers(void){
	return this->servers;
}

std::vector<t_listen>		Config::getAllListenDir(void){
	std::vector<t_listen>	listen;
	for (size_t i = 0; i < this->servers.size(); i++){
		t_listen t(servers[i]->gethostPort());
		listen.push_back(t);
	}
	return listen;
}

std::list<std::list<std::string> >	Config::getAllServerNames(void){
	std::list<std::list<std::string> > servNames;
	for (size_t i = 0; i < this->servers.size(); i++){
		servNames.push_back(this->servers[i]->getServerName());
	}
	return servNames;
}

/*---- Operators -------*/

Config	&Config::operator=(const Config &obj){
	this->servers = obj.servers;
	return *this;
}

/*---- Member Functions ----*/

bool exepectedTokens(std::string value)
{
	const char* exepectedTokens[] = {"root", "alias", "allow_methods", 
	"client_body_buffer_size","cgi_pass", "server_names", "listen", 
	"location", "index", "error_pages", "server", "{", "}", "autoindex", "upload_store", "return"};
	for (size_t i = 0; i < 16; i++){
		if (!value.compare(exepectedTokens[i]))
			return true;
	}
	return false;
}

configFile		Config::slitTokens(configFile con, std::string delim)
{
	configFile	tokens;
	bool		isKey;
	for (size_t i = 0; i < con.size(); i++)
	{
		isKey = 1;
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
			if (isKey && !exepectedTokens(token)){
				puts("here");
				std::cout << "Error: " << token << " is not a valid token" << std::endl;
				throw Config::FileNotWellFormated();
			}
			isKey = 0;
			tokens.push_back(token);
			start = str.find_first_not_of(delim, end);
			if (start == std::string::npos)
				break ;
		}
	}
	return tokens;
}

configFile				Config::readFile(const char *fileName){
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
	return	(Config::slitTokens(con, " "));
}

void					Config::parseFile(const char *fileName)
{
	configFile		confFile;
	unsigned int	confSize;

	if (open(fileName, O_RDONLY) < 0)
		throw Config::FileCorrupted();
	confFile = Config::readFile(fileName);
	curlLevel(confFile);
	confSize = confFile.size();
	for (unsigned int i = 0; i < confSize; i++){
		if (!confFile[i].compare("server")){
			++i;
			if (!confFile[i].compare("{"))
				i = Config::parseServer(confFile, i);
		}
		else
			throw	Config::FileNotWellFormated();
	}
	checkForDup();
}

configFile::iterator	Config::curlLevel(configFile con){
	configFile::iterator it, ite;
	it = con.begin();
	ite  = con.end();
	size_t curlLvl = 0;
	while (it != ite){
		if (*it == "{")
			curlLvl++;
		else if (*it == "}")
			curlLvl--;
		it++;
	}
	if (curlLvl != 0)
		throw Config::FileNotWellFormated();
	return it;
}

void				Config::checkForDup(void){
	std::vector<t_listen>				listens = this->getAllListenDir();

	for (size_t i = 0; i < listens.size(); i++){
		for (size_t j = i + 1; j < listens.size(); j++){
			if (listens[i].port == listens[j].port && listens[i].host == listens[j].host)
				throw Config::FileNotWellFormated();
		}
	}
}

size_t		Config::parseServer(configFile con, unsigned int &index){
	serverConfig *server = new serverConfig();
	bool isLocation = 0;
	Ptr values[10] = {&serverConfig::serverName, &serverConfig::root,
	&serverConfig::listen, &serverConfig::parseLocation, &serverConfig::allowMethods,
	&serverConfig::index, &serverConfig::errorPages, &serverConfig::autoIndex, &serverConfig::limitBodySize,  &serverConfig::redirect};
	while (true)
	{
		index++;
		for (size_t i = 0; i < 10; i++)
		{
			if (i == 3)
				isLocation = 1;
			if (index >= con.size())
				break;
			if (con[index] == keys[i]){
				index = (server->*values[i])(*server, con, index);
			}
		}
		if (index >= con.size() || (con[index] == "}" && !isLocation) || !con[index].compare("server"))
			break;
	}
	this->servers.push_back(server);
	index--;
	return index;
}
