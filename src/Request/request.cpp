/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:22:03 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/19 22:00:57 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

/*------ Constructors ------*/

request::request()
{
}

request::~request()
{
}

/*------ Accessors ------*/

std::string								request::getMethod(){
    return     _method;
}
std::string								request::getRequestURI(){
    return     _requestURI;
}
std::string								request::getVersion(){
    return      _version;
}
std::map<std::string, std::string>		request::getHeaders(){
    return      _headers;
}

/*------ Member Functions ------*/

int			request::getFirstLine(std::string buff){
    std::string delim("\n");
    int end = buff.find_first_of(delim, 0);
    (void)end;
    return 1;
}

int        request::startParsing(std::string buff){
    this->getFirstLine(buff);
    return 1;
}

