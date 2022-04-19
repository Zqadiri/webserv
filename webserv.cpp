/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 11:05:42 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/19 16:10:03 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/webserv.hpp"
#include "./src/servers.hpp"
#include "./src/Request/request.hpp"


int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		Servers  serv;
		try{
			serv.conf(argv);
			serv.setup();
			//!	just for testing
				request req;
				std::string raw_request = "GET / HTTP/1.1\r\n"
				"Host: localhost:8080\r\n"
				"Connection: keep-alive\r\n"
				"Upgrade-Insecure-Requests: 1\r\n"
				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
				"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/604.5.6 (KHTML, like Gecko) Version/11.0.3 Safari/604.5.6\r\n"
				"Accept-Language: en-us\r\n"
				"DNT: 1\r\n"
				"Accept-Encoding: gzip, deflate\r\n"
				"\r\n"
				"Usually GET requests don\'t have a body\r\n"
				"But I don\'t care in this case :)";
				req.startParsing(raw_request);
			//!
			// serv.run();
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
