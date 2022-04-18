/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/18 17:22:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/18 22:53:43 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ! simple program to parse an http request 
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

enum    Method
{
	GET, 
	POST, 
	DELETE
};

typedef	struct Header
{
	/* data */
}	;


typedef struct Request {
	enum Method method;  //? possible methods are GET POST PUT
	char *version;
	char *url;
	struct Header *headers;
	char *body;
}               Request;

void    parseRequest(char *buffer)
{
	printf(" -> %s\n",buffer );
	
}

int main(int argc, char const *argv[])
{
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char * hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0){
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
			perror("In accept");
			exit(EXIT_FAILURE);
		}
		char buffer[30000] = {0};
		valread = read( new_socket , buffer, 30000);
		parseRequest(buffer);
		write(new_socket , hello , strlen(hello));
		printf("------------------Hello message sent-------------------");
		close(new_socket);
		break;
	}
	return 0;
}
