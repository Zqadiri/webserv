/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:31:24 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/11 14:26:00 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "../includes/webserv.hpp"
#include "./serverConfig.hpp"

#define BUFFER_SIZE	2048

class Config
{
	private:
		std::vector<serverConfig*>	servers;
	public:
		Config();
		~Config();
		Config(const Config&);
		Config	&operator=(const Config&);

		void								parseFile(const char *fileName);
		configFile							readFile(const char *fileName);
		configFile							slitTokens(configFile, std::string);
		size_t								parseServer(configFile , unsigned int&);
		std::string							removeSpace(std::string);
		configFile::iterator				curlLevel(configFile);
		void								checkForDup();
		
		const std::vector<serverConfig*>			&getServers(void);
		std::vector<t_listen>				getAllListenDir(void);
		std::list<std::list<std::string> >	getAllServerNames(void);

		class	FileCorrupted : public std::exception{
			public:
				virtual const char* what() const throw();
		};
		class	FileNotWellFormated : public std::exception{
			public:
				virtual const char* what() const throw();
		};

		//* print function
		void	print();
};

#endif