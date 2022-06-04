/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:38:06 by zqadiri           #+#    #+#             */
/*   Updated: 2022/06/04 22:35:17 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfig.hpp"

const char* keys[] = {
	"server_names", 
	"root", 
	"listen", 
	"location", 
	"allow_methods",
	"index", 
	"error_pages",
	"autoindex",
	"client_body_buffer_size",
	"return"
};

const char* locationKeys[] = {
	"root",
	"alias", 
	"allow_methods", 
	"client_body_buffer_size", 
	"index",
	"cgi_pass",
	"autoindex",
	"upload_store"
};

bool notAValue(std::string value){
	for (size_t i = 0; i < 10; i++){
		if (!value.compare(keys[i]))
			return true;
	}
	return false;
}

bool notAValueL(std::string value){
	for (size_t i = 0; i < 8; i++){
		if (!value.compare(locationKeys[i]))
			return true;
	}
	return false;
}

/*---- Constructors & Destructor ------*/

serverConfig::serverConfig() : _root(""), _index(""), _hostPort(t_listen()){
	_hostPort.host = 0;
	_redirect.code = 0;
	_redirect.path = "";
	_autoindex = false;
	_hostPort.port = -1;
	_limitBodySize = -1;
}

serverConfig::~serverConfig(){
}

serverConfig::serverConfig(const serverConfig &obj){
	*this = obj;
}

/*---- Operators -------*/

serverConfig	&serverConfig::operator=(const serverConfig &obj){
	this->_server_name = obj._server_name;
	this->_root = obj._root;
	this->_index = obj._index;
	this->_hostPort = obj._hostPort;
	this->_errorPages.code = obj._errorPages.code;
	this->_errorPages.path = obj._errorPages.path;
	this->_allow_methods = obj._allow_methods;
	this->_locations = obj._locations;
	return *this;
}

/*---- Accessors ----*/

const std::list<std::string>	&serverConfig::getAllowMethods(void) const{ return this->_allow_methods; }
const t_error_pages				&serverConfig::getErrorsPages(void) const{ return this->_errorPages; }
const t_listen				 	&serverConfig::gethostPort(void) const{ return this->_hostPort; }
int								serverConfig::getlimitBodySize(void) const {return this->_limitBodySize; }
const std::list<std::string>	&serverConfig::getServerName(void) const { return this->_server_name;}
const std::vector<_location>	&serverConfig::getLocations(void) const{ return this->_locations;}
bool							serverConfig::getAutoIndex(void) const { return this->_autoindex; }

/*---- Member Functions ----*/

void		checkForValue(std::string value)
{
	if (notAValue(value)){
		std::cerr << RED << "every key should be followed by value!" << RESET << std::endl;
		exit (EXIT_FAILURE);
	}
}

unsigned int	serverConfig::serverName(serverConfig &serv, configFile con, unsigned int &index)
{
	index++;
	checkForValue(con[index]);
	while (!notAValue(con[index])){
		serv._server_name.push_back(con[index]);
		index++;
	}
	index--;
	return index;
}

unsigned int	serverConfig::location(_location &l, configFile con, unsigned int &index){
	while (true)
	{
		if (!con[index].compare("root")){ 
			index++;
			l._root = con[index];
			index++;
		}
		else if (!con[index].compare("upload_store")){
			index++;
			l._uploadStore = con[index];
			index++;
		}
		else if (!con[index].compare("index")){ 
			index++;
			l._index = con[index];
			index++;
		}
		else if (!con[index].compare("alias")){
			l._alias = true;
			index++;
		}
		else if (!con[index].compare("allow_methods")){
			index++;
			while (!notAValueL(con[index])){
				l._allow_methods.push_back(con[index]);
				index++;
			}
		}
		else if (!con[index].compare("client_body_buffer_size")){
			index++;
			l._limitBodySize = stoi(con[index++]);
		}
		else if (!con[index].compare("cgi_pass")){
			index++;
			l._pathCGI = con[index++];
		}
		else if (!con[index].compare("autoindex")){
			index++;
			if (con[index].compare("on") && con[index].compare("off"))
				throw "Autoindex";
			else if (!con[index].compare("on"))
				l._autoindex = 1;
		}
		else
			index++;
		if (!con[index].compare("}"))
			break;
	}
	return index;
}
 
unsigned int	serverConfig::autoIndex(serverConfig &serv, configFile con, unsigned int& index)
{
	index++;
	if (con[index].compare("on") && con[index].compare("off"))
		throw "Autoindex";
	else if (!con[index].compare("on"))
		serv._autoindex = true;
	return index++;
}

unsigned int	serverConfig::parseLocation(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	_location l;
	l._path = con[index++];
	l._alias = false;
	if (!con[index++].compare("{"))
		index = location(l, con, index);
	else
		throw "MSG";
	serv._locations.push_back(l);
	return index;
}

uint32_t convert( const std::string& ipv4Str ){
	std::istringstream iss( ipv4Str );
	uint32_t ipv4 = 0;
	
	for( uint32_t i = 0; i < 4; ++i ) {
		uint32_t part;
		iss >> part;
		if ( iss.fail() || part > 255 ) {
			throw std::runtime_error( "Invalid IP address - Expected [0, 255]" );
		}
		if ( i != 3 ) {
			char delimiter;
			iss >> delimiter;
			if ( iss.fail() || delimiter != '.' ) {
				throw std::runtime_error( "Invalid IP address - Expected '.' delimiter" );
			}
		}
		ipv4 = inet_addr(ipv4Str.c_str());
	}
	return ipv4;
}

unsigned int	serverConfig::listen(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	checkForValue(con[index]);
	if (_hostPort.host != 0 && _hostPort.port != -1)
		throw std::runtime_error("two Listen directives");
	_hostPort.host = inet_addr("192.168.0.1");
	_hostPort.port = 80;
	std::string delim(":");
	size_t end = con[index].find_first_of(delim, 0);
	if (std::string(con[index].substr(0, end)).compare("localhost"))
		serv._hostPort.host = convert(con[index].substr(0, end));
	try {
		if (end != std::string::npos){
			end++;
			serv._hostPort.port = stoi(con[index].substr(end, con[index].find_first_of(delim, end)));
		}
	}
	catch (std::exception &r){
		std::cout << RED << "Bad Port" << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	return index++;
}


unsigned int	serverConfig::root(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	checkForValue(con[index]);
	serv._root = con[index];
	return index;
}

unsigned int	serverConfig::index(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	checkForValue(con[index]);
	serv._index = con[index];
	return index;
}

unsigned int	serverConfig::redirect(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	serv._redirect.code = stoi(con[index]);
	index++;
	serv._redirect.path = con[index];
	index++;
	return index;
}

unsigned int	serverConfig::limitBodySize(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	checkForValue(con[index]);
	try {
		serv._limitBodySize = stoi(con[index]);
	}
	catch (std::exception &r){
		std::cout << "Bad Limit" << std::endl;
	}
	return index;
}

unsigned int	serverConfig::errorPages(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	checkForValue(con[index]);
	serv._errorPages.code = stoi(con[index++]);
	serv._errorPages.path = con[index];
	index--;
	return index;
}

unsigned int	serverConfig::allowMethods(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	checkForValue(con[index]);
	while (!notAValue(con[index])){
		serv._allow_methods.push_back(con[index]);
		index++;
	}
	index--;
	return index;
}