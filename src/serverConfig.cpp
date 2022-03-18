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

size_t	serverConfig::serverName(const serverConfig &serv, configFile con)
{
	puts("server_name");
}


