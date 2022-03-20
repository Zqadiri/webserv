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

// const char* keys[] = {
// 	"server_names", 
// 	"listen", 
// 	"allow_methods",
// 	"location", 
// 	"root", 
// 	"index", 
// 	"error_pages"
// };

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
		if (!value.compare(keys[i]))
			return true;
	}
	return false;
}


unsigned int	serverConfig::serverName(serverConfig &serv, configFile con, unsigned int &index)
{
	// puts("server_names");
	while (!notAValue(con[index]))
	{
		serv._server_name.push_back(con[index]);
		index++;
	}
	return index;
}

unsigned int	serverConfig::location(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("location");
	// while ()
	return 0 ;
}

unsigned int	serverConfig::listen(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("listen");
	return 0 ;
}

unsigned int	serverConfig::root(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("root");

	return 0 ;
}

unsigned int	serverConfig::index(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("index");
	return 0 ;
}

unsigned int	serverConfig::errorPages(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("errorPages");
	return 0 ;
}

unsigned int	serverConfig::allowMethods(serverConfig &serv, configFile con, unsigned int &index)
{
	puts("allowMethods");
	return 0 ;
}