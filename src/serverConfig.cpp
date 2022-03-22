/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:38:06 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/18 11:41:34 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfig.hpp"

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
}

serverConfig::~serverConfig(){
}

serverConfig::serverConfig(const serverConfig &obj){
}

/*---- Operators -------*/

serverConfig	&serverConfig::operator=(const serverConfig&){
	
	return *this;
}

/*---- Member Functions ----*/

bool notAValue(std::string value)
{
	for (size_t i = 0; i < 7; i++)
	{
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
	puts("[serverName]");
	index++;
	while (!notAValue(con[index]))
	{
		serv._server_name.push_back(con[index]);
		index++;
	}
	for (std::list<std::string>::iterator i = serv._server_name.begin(); i != serv._server_name.end(); ++i)
		std::cout << "-> " <<*i << std::endl;
	return index;
}

unsigned int	serverConfig::location(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("[location]");
	index++;
	_location l;
	l._path = con[index++];
	l._alias = false;
	if (!con[index++].compare("{"))
	{
		while (true)
		{
			if (!con[index].compare("root"))
			{
				index++;
				l._root = con[index];
				index++;
			}
			else if (!con[index].compare("alias"))
			{
				l._alias = true;
				index++;
			}
			else if (!con[index].compare("allow_methods"))
			{
				index++;
				while (!notAValueL(con[index]))
				{
					l._allow_methods.push_back(con[index]);
					index++;
				}
			}
			else
				index++;
			if (!con[index].compare("}"))
				break;
		}
	}
	else
		throw "MSG";
	std::cout << "-> path " << l._path << std::endl;
	std::cout << "-> root " << l._root << std::endl;
	std::cout << "-> alias " << l._alias << std::endl;
	std::cout << "-> allow_methods : " << std::endl;
	for (std::list<std::string>::iterator i = l._allow_methods.begin(); i != l._allow_methods.end(); ++i)
		std::cout << "-> " <<*i << std::endl;
	return index;
}

unsigned int	serverConfig::listen(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("[listen]");
	index++;
	std::string delim(":");
	size_t end = con[index].find_first_of(delim, 0);
	serv._host = con[index].substr(0, end);
	try {
		end++;
		serv._port = stoi(con[index].substr(end, con[index].find_first_of(delim, end)));
	}
	catch (std::exception &r){
		std::cout << "Bad Port" << std::endl;
	}           
	std::cout << "->" << serv._host << std::endl;
	std::cout << "->" << serv._port << std::endl;
	return index++;
}

unsigned int	serverConfig::root(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("[root]");
	index++;
	serv._root = con[index];
	std::cout << "-> " << serv._root << std::endl;
	return index;
}

unsigned int	serverConfig::index(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("[index]");
	index++;
	serv._index = con[index];
	std::cout << "-> " << serv._index << std::endl;
	return index;
}

unsigned int	serverConfig::errorPages(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("[errorPages]");
	index++;
	while (!notAValue(con[index]))
	{
		serv._error_pages.push_back(con[index]);
		index++;
	}
	for (std::list<std::string>::iterator i = serv._error_pages.begin(); i != serv._error_pages.end(); ++i)
		std::cout << "-> " <<*i << std::endl;
	return index;
}

unsigned int	serverConfig::allowMethods(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("[allowMethods]");
	index++;

	while (!notAValue(con[index]))
	{
		serv._allow_methods.push_back(con[index]);
		index++;
	}
	for (std::list<std::string>::iterator i = serv._allow_methods.begin(); i != serv._allow_methods.end(); ++i)
		std::cout << "-> " <<*i << std::endl;
	return index;
}