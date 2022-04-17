/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 13:51:02 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/15 21:31:46 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

// Libft
#include "../libft/libft.h"

// CPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <list>
#include <iterator>

// C 
#include <unistd.h>
#include <fcntl.h>

// Network 
#include <sys/socket.h>
#include <netinet/in.h>

// Colors
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"

typedef	std::vector<std::string>	configFile;

typedef struct s_listen{
	unsigned int    host;//
	int             port;
}   t_listen;

#endif
