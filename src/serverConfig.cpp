/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:38:06 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/17 02:44:34 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfig.hpp"

static bool isNested = 0;

const char* keys_[] = {
	"server_names", 
	"root", 
	"listen", 
	"location", 
	"allow_methods",
	"index", 
	"error_pages"
};

const char* locationKeys[] = {
	"root", 
	"alias", 
	"allow_methods", 
	"client_body_buffer_size", 
	"index",
	"cgi_pass", 
};

/*---- Constructors & Destructor ------*/

serverConfig::serverConfig(){
	_host = inet_addr("192.168.0.1");
	_port = 80;
}

serverConfig::~serverConfig(){
}

serverConfig::serverConfig(const serverConfig &obj){
	(void)obj;
}

/*---- Operators -------*/

serverConfig	&serverConfig::operator=(const serverConfig&){
	return *this;
}

/*---- Member Functions ----*/

bool notAValue(std::string value)
{
	for (size_t i = 0; i < 7; i++){
		if (!value.compare(keys_[i]))
			return true;
	}
	return false;
}

bool notAValueL(std::string value)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (!value.compare(locationKeys[i]))
			return true;
	}
	return false;
}

unsigned int	serverConfig::serverName(serverConfig &serv, configFile con, unsigned int &index)
{
	index++;
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
			l._limitBodySize = stoi(con[index++]); //! exception may happen [abort]
		}
		else if (!con[index].compare("cgi_pass")){
			index++;
			l._pathCGI = con[index++];
		}
		else
			index++;
		if (!con[index].compare("location")){
			isNested = 1;
		}
		if (!con[index].compare("}"))
			break;
	}
	return index;
}

unsigned int	serverConfig::parseLocation(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	_location l;
	l._path = con[index++];
	l._alias = false;
	l._limitBodySize = -1;
	if (!con[index++].compare("{"))
		index = location(l, con, index);
	else
		throw "MSG";
	if (!isNested)
		serv._locations.push_back(l);
	else {
		l._nestedLocations.push_back(l);
		isNested = 0;
	}
	return index;
}

// localhost
// ip address -> always to 192.168.0.1
// no port  -> default 80

uint32_t convert( const std::string& ipv4Str ) //! convert and check for errors
{
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
	std::string delim(":");
	size_t end = con[index].find_first_of(delim, 0);
	if (end == std::string::npos || std::string(con[index].substr(0, end)).compare("localhost")){ //! no port || localhost
		serv._host = convert(con[index].substr(0, end));
		return index++;
	}
	try {
		end++;
		serv._port = stoi(con[index].substr(end, con[index].find_first_of(delim, end)));
	}
	catch (std::exception &r){
		std::cout << "Bad Port" << std::endl;
	}
	return index++;
}

unsigned int	serverConfig::root(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	serv._root = con[index];
	return index;
}

unsigned int	serverConfig::index(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	serv._index = con[index];
	return index;
}

unsigned int	serverConfig::errorPages(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	while (!notAValue(con[index])){
		serv._error_pages.push_back(con[index]);
		index++;
	}
	index--;
	return index;
}

unsigned int	serverConfig::allowMethods(serverConfig &serv, configFile con, unsigned int &index){
	index++;
	while (!notAValue(con[index])){
		serv._allow_methods.push_back(con[index]);
		index++;
	}
	index--;
	return index;
}