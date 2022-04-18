/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:05:42 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/18 01:39:12 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/webserv.hpp"
#include "./src/servers.hpp"

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		Servers  serv;
		try{
			serv.conf(argv);
			serv.setup();
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
