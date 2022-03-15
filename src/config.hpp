/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:24 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/15 18:59:10 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "../includes/webserv.hpp"

#define BUFFER_SIZE	1024

typedef	std::vector<std::string>	configFile;

class Config
{
	private:
		// std::vector<std::string>	configFile;
	public:
		Config();
		~Config();
		Config(const Config&);
		Config	&operator=(const Config&);
		
		void		parseFile(const char *fileName);
		configFile	readFile(const char *fileName);
		configFile	slitTokens(configFile, std::string);
		
		class	FileCorrupted : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		class	FileNotWellFormated : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};
#endif