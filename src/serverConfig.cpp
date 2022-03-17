/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:38:06 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/16 15:56:09 by zqadiri          ###   ########.fr       */
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

// std::vector<std::string>::iterator    curlLevel(std::vector<std::string> con,  unsigned int &index)
// {
    // return it;
// }

size_t		serverConfig::parseServer(std::vector<std::string> con, unsigned int &index)
{
	puts("parseServer");
	// std::vector<std::string>::iterator it = curlLevel(con, index);
  	std::vector<std::string>::iterator it, ite;
	size_t	start = index;
	it = con.begin() + index;
	ite  = con.end();
	size_t curlLvl = 1;
	while (it != ite)
	{
		if (*it == "server")
			break;
		
		index++;	
		it++;
	}
	std::cout << *it << std::endl;
	return index--;
}

