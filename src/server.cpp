/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:10:13 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/15 11:51:37 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*---- Constructors & Destructor ------*/

Server::Server(){
}

Server::~Server(){
}

Server::Server(const Server &sv){
    *this = sv;
}

/*---- Operators -------*/

Server	&Server::operator=(const Server&){
    
    return *this;    
}

/*---- Member Functions ----*/

void    Server::conf(char **argv){
   config.parseFile(argv[1]);
}

