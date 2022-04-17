/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:27 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/17 02:48:41 by zqadiri          ###   ########.fr       */
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

/*---- Accessors -------*/

std::vector<serverConfig*>	Config::getServers(void){
	return this->servers;
}

/*---- Operators -------*/

Config	&Config::operator=(const Config &obj){
	// if (*this == obj) //! check for self assign [overload ==]
	// 	return *this;
	this->servers = obj.servers;
	return *this;
}

/*---- Member Functions ----*/

bool exepectedTokens(std::string value) //! use the one in serverConfig
{
	const char* exepectedTokens[] = {"root", "alias", "allow_methods", 
	"client_body_buffer_size","cgi_pass", "server_names", "listen", 
	"location", "index", "error_page", "server", "{", "}"};
	for (size_t i = 0; i < 13; i++)
	{
		if (!value.compare(exepectedTokens[i]))
			return true;
	}
	return false;
}

std::string		Config::removeSpace(std::string init)
{
	std::string ret;
	for (size_t i = 0; i < init.length(); ++i)
	{
		if (!isspace(init[i]))
			ret.push_back(init[i]);
	}
	return ret;
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

//! add an init to set all fields to default values 
void		Config::parseFile(const char *fileName)
{
	configFile		confFile;
	unsigned int	confSize;

	if (open(fileName, O_RDONLY) < 0)
		throw Config::FileCorrupted();
	confFile = Config::readFile(fileName);
	//! check for unclosed curl
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

typedef unsigned int (serverConfig::*Ptr)(serverConfig&, configFile, unsigned int&);

size_t		Config::parseServer(configFile con, unsigned int &index){
	serverConfig *server = new serverConfig();
	bool isLocation = 0;
	Ptr values[7] = {&serverConfig::serverName, &serverConfig::root,
	&serverConfig::listen, &serverConfig::parseLocation, &serverConfig::allowMethods,
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
				index = (server->*values[i])(*server, con, index);
		}
		if (index >= con.size() || (con[index] == "}" && !isLocation) || !con[index].compare("server"))
			break;
	}
	this->servers.push_back(server);
	index--;
	return index; //! return index to the last colla 
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
		std::cout << this->servers[i]->_host << std::endl;
		std::cout << this->servers[i]->_port << std::endl;
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