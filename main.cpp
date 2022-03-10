/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 15:53:11 by zqadiri           #+#    #+#             */
/*   Updated: 2022/03/10 19:41:54 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main (void)
{
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(9999);

	int tcp_socket;
	if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) //* create a socket (IPv4, TCP)
	{
		std::cerr << "cannot create socket" << std::endl; 
		return 0;
	}
	else
	{
		int bind_ret;
		if ((bind_ret = bind(tcp_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) == -1) //* assign a transport address to the socket
		{
			std::cerr << "cannot bind socket" << std::endl; 
			return 0;
		}
		if (listen(tcp_socket, 10) < 0) {
			std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}

		static int lenadd = sizeof(sockaddr);
		int connection = accept(tcp_socket, (struct sockaddr*)&sockaddr, (socklen_t*)&lenadd);
		if (connection < 0) {
			std::cout << "Failed to grab connection. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}

		char buffer[100]; 
		static int  bytesRead = read(connection, buffer, 100); //* Read from the connection
		std::cout << "The message was: " << buffer;

		// Send a message to the connection
		std::string response = "Good talking to you\n";
		send(connection, response.c_str(), response.size(), 0);

		// Close the connections
		close(connection);
		close(tcp_socket);
	}
	return 0;
}