/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:51:02 by zqadiri           #+#    #+#             */
/*   Updated: 2022/06/03 14:42:30 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string			removeSpace(std::string init){
	std::string ret;
	for (size_t i = 0; i < init.length(); ++i){
		if (!isspace(init[i]))
			ret.push_back(init[i]);
	}
	return ret;
}

std::string	to_string(size_t n)
{
	std::stringstream tmp;
	tmp << n;
	return tmp.str();
}

std::string			getNextLine(const std::string &buff, size_t &cursor)
{
	std::string		ret;
	size_t			i;

	if (cursor == std::string::npos)
		return "";
	i = buff.find_first_of('\n', cursor);
	ret = buff.substr(cursor, i - cursor);
	if (ret[ret.size() - 1] == '\r'){
		ret.resize(ret.size() - 1);
	}
	if (i != std::string::npos)
		i++;
	cursor = i;
	return ret;
}

std::string		randomFileName(void)
{
	std::string fileName("./tmp/");
	time_t t = time(0);
	struct tm *now = localtime(&t);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", now);
	fileName += buffer;
	return fileName;
}