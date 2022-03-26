/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:05:42 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/26 14:34:21 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/webserv.hpp"
#include "./src/server.hpp"

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		Server  serv;
		try
		{
			serv.conf(argv);
		}
		catch(const std::exception& e)
		{
			std::cerr << RED << e.what() << '\n';
		}
		// system("leaks webserv");
	}
	else 
		std::cout << RED << "ARG ERROR" << std::endl;
	return EXIT_SUCCESS;
}
