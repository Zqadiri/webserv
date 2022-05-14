/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:27 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/14 12:46:57 by zqadiri          ###   ########.fr       */
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
	"location", "index", "error_page", "server", "{", "}", "autoindex"};
	for (size_t i = 0; i < 14; i++){
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
			if (isKey && !exepectedTokens(token))
				throw Config::FileNotWellFormated();
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
	print();
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
	std::list<std::list<std::string> > 	servNames = this->getAllServerNames();

	size_t  k = 0;
	std::map<std::list<std::string> ,int> pairs;
	for (std::list<std::list<std::string> >::iterator i = servNames.begin(); 
					i != servNames.end(); i++)
	{
		std::list<std::string> inerList = *i;
		pairs.insert(std::make_pair(*i, listens.at(k).port));
		k++;
	}
	k = 0;
	for (std::map<std::list<std::string> ,int>::iterator i = pairs.begin(); 
					i != pairs.end(); ++i)
	{
		if (k >= pairs.size() - 1)
			break;
		std::list<std::string> inerList = i->first;
		std::map<std::list<std::string>,int >::iterator tmp = i;
		tmp++;
		std::list<std::string> NinerList = tmp->first;
		for (std::list<std::string>::iterator it = inerList.begin(); 
					it != inerList.end(); ++it){
			for (std::list<std::string>::iterator itn = NinerList.begin(); 
				itn != NinerList.end(); ++itn){
				if (*it == *itn && i->second == tmp->second)
					throw "duplicate Servers";
			}
		}
		k++;
	}
}

size_t		Config::parseServer(configFile con, unsigned int &index){
	serverConfig *server = new serverConfig();
	bool isLocation = 0;
	Ptr values[8] = {&serverConfig::serverName, &serverConfig::root,
	&serverConfig::listen, &serverConfig::parseLocation, &serverConfig::allowMethods,
	&serverConfig::index, &serverConfig::errorPages, &serverConfig::autoIndex};
	while (true)
	{
		index++;
		for (size_t i = 0; i < 8; i++)
		{
			if (i == 3)
				isLocation = 1;
			if (index >= con.size())
				break; 
			if (con[index] == keys[i])
			{
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

void	Config::print(){
	std::cout << "[Servers n: " << this->servers.size() << "]"<< std::endl;
	for (size_t i = 0; i < this->servers.size(); i++)
	{
		std::cout << "-------------------------------------" << std::endl;
		puts("[serverName]");
		for (std::list<std::string>::iterator it = this->servers[i]->_server_name.begin(); 
				it != this->servers[i]->_server_name.end(); ++it)
			std::cout << " > " << *it << std::endl;
		puts("[root]");
		std::cout << this->servers[i]->_root << std::endl;
		puts("[index]");
		std::cout << this->servers[i]->_index << std::endl;
		puts("[listen]");
		std::cout << this->servers[i]->_hostPort.host << std::endl;
		std::cout << this->servers[i]->_hostPort.port << std::endl;
		puts("[autoindex]");
		std::cout << this->servers[i]->_autoindex << std::endl;
		puts("[errorPages]");
		for (std::list<std::string>::iterator it = this->servers[i]->_error_pages.begin(); 
				it != this->servers[i]->_error_pages.end(); ++it)
			std::cout << " > " << *it << std::endl;
		puts("[allow_methods]");
		for (std::list<std::string>::iterator it = this->servers[i]->_allow_methods.begin(); 
				it != this->servers[i]->_allow_methods.end(); ++it)
		std::cout << " > " <<*it << std::endl;
		std::cout << "[locations n: " << this->servers[i]->_locations.size()  << "]"<< std::endl;
		for (size_t j = 0; j < this->servers[i]->_locations.size(); j++)
		{
			std::cout << "[location "<< j << "]"<< std::endl;
			std::cout << "path " << this->servers[i]->_locations[j]._path << std::endl;
			std::cout << "root " << this->servers[i]->_locations[j]._root << std::endl;
			std::cout << "index " << this->servers[i]->_locations[j]._index << std::endl;
			std::cout << "limitBodySize " << this->servers[i]->_locations[j]._limitBodySize << std::endl;
			std::cout << "autoindex " << this->servers[i]->_locations[j]._autoindex << std::endl;
			std::cout << "CGIpass " << this->servers[i]->_locations[j]._pathCGI << std::endl;
			std::cout << "alias " << this->servers[i]->_locations[j]._alias << std::endl;
			puts("allow_methods");
			for (std::list<std::string>::iterator it = this->servers[i]->_locations[j]._allow_methods.begin(); 
				it != this->servers[i]->_locations[j]._allow_methods.end(); ++it)
					std::cout << " > " << *it << std::endl;
			std::cout << " >> [nestedLocation "<< this->servers[i]->_locations[j]._nestedLocations.size() << "]"<< std::endl;
			for (size_t  k = 0; k < this->servers[i]->_locations[j]._nestedLocations.size(); k++){
				std::cout << this->servers[i]->_locations[j]._nestedLocations[k]._path << std::endl;
				std::cout << this->servers[i]->_locations[j]._nestedLocations[k]._alias << std::endl;
				std::cout << this->servers[i]->_locations[j]._nestedLocations[k]._pathCGI << std::endl;
				std::cout << this->servers[i]->_locations[j]._nestedLocations[k]._index << std::endl;
				std::cout << this->servers[i]->_locations[j]._nestedLocations[k]._root << std::endl;
			}
		}
	}
}