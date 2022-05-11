/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:05:42 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/10 17:10:33 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/webserv.hpp"
#include "./src/servers.hpp"
#include "./src/Request/request.hpp"
#include <set>
int ft_main(int argc, char *argv[])
{
	if (argc == 2)
	{
		Servers  serv;
		try{
			serv.conf(argv);
			serv.setup();
			serv.run();
		}
		catch(const std::exception& e)
		{
			std::cerr << RED << e.what() << '\n';
		}
	}
	else 
		std::cout << RED << "ARG ERROR" << std::endl;
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	ft_main(argc, argv);
	// system("leaks webserv");
	return 1;
}
