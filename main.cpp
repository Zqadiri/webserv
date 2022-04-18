/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 15:53:11 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/18 17:12:04 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "./includes/webserv.hpp"

// #include <vector>
// int main (int argc, char *argv[])
// {
// 	if (argc == 1)	// !2
// 	{
// 		sockaddr_in sockaddr;
// 		sockaddr.sin_family = AF_INET;
// 		sockaddr.sin_addr.s_addr = INADDR_ANY;
// 		sockaddr.sin_port = htons(9999);

// 		int tcp_socket;
// 		if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) //* create a Stream socket (IPv4, TCP)
// 		{
// 			std::cerr << "cannot create socket" << std::endl; 
// 			return 0;
// 		}
// 		else
// 		{
// 			int bind_ret;
// 			if ((bind_ret = bind(tcp_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) == -1) 
// 			{
// 				//* assign a transport address to the socket
// 				std::cerr << "cannot bind socket" << std::endl; 
// 				return 0;
// 			}
// 			if (listen(tcp_socket, 10) < 0) {
// 				std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
// 				exit(EXIT_FAILURE);
// 			}

// 			static int lenadd = sizeof(sockaddr);
// 			int connection = accept(tcp_socket, (struct sockaddr*)&sockaddr, (socklen_t*)&lenadd);
// 			if (connection < 0) {
// 				std::cout << "Failed to grab connection. errno: " << errno << std::endl;
// 				exit(EXIT_FAILURE);
// 			}

// 			char buffer[1000]; 
// 			static int  bytesRead = read(connection, buffer, 100); //* Read from the connection
// 			std::cout << "The message was: " << buffer;

// 			std::string response = "Good talking to you\n";
// 			send(connection, response.c_str(), response.size(), 0); //* Send a message to the connection

// 			close(connection); //* Close the connections
// 			close(tcp_socket);
// 		}
// 	}
// 	else {
// 		std::cout << RED << "ARG ERROR" << std::endl;
// 	}
// 	return 0;
// }


// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Only this line has been changed. Everything is same.
    char * hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
    return 0;
}