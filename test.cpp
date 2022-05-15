
// CPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <list>
#include <iterator>
#include <sstream>
#include <algorithm>

// C 
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

// Network 
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    struct sockaddr_in		_addr;
    int						_fd;
    fd_set                  _fd_set;
    int                     f = 1;
    int                     _max_fd;
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &f, sizeof(int));

    fcntl(_fd, F_SETFL, O_NONBLOCK);

    bzero((char *)&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(0);
	_addr.sin_port = htons(8080);
    bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr));
    listen(_fd, 10);
    _max_fd = _fd;
    fd_set write_set;
    FD_ZERO(&write_set);
    FD_ZERO(&_fd_set);
    FD_SET(_fd, &_fd_set);
    std::cout << "_fd: " << _fd << std::endl;
    while (1)
    {
        fd_set fset;
		fd_set wset;
        FD_ZERO(&fset);
		FD_ZERO(&wset);
		FD_COPY(&_fd_set, &fset);
		FD_COPY(&write_set, &wset);

        int selected = select(_max_fd + 1, &fset, &wset, NULL, NULL);
        std::cout << "selected: " << selected << std::endl;
        std::cout << "max_fd: " << _max_fd << std::endl;
        if (selected == -1)
        {
			std::cout << "select error" << std::endl;
		}
        if (selected > 0)
        {
            int socket = accept(_fd, NULL, NULL);
            std::cout << "accept" << std::endl;
            if (socket != -1)
            {
                std::cout << socket << std::endl;
                fcntl(socket, F_SETFL, O_NONBLOCK);
                FD_SET(socket, &_fd_set);
                if (socket > _max_fd)
                    _max_fd = socket;
            }
            std::cout << "send check" << std::endl;
            for (int i = 0; i < _max_fd + 1; ++i)
            {
                if (i == _fd)
                    continue;
                if (FD_ISSET(i, &wset))
                {
                    std::cout << i << std::endl;
                    int ret;
                    std::string buf;
                    buf = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nContent-Location: /index.html\r\nContent-Type: text/html\r\nDate: Tue, 19 Apr 2022 19:58:38 GMT\r\nLast-Modified: Tue, 19 Apr 2022 19:58:38 GMT\r\nServer: Webserv/1.0.0 (Unix)\r\nTransfer-Encoding: identity\r\n\r\n";
                    // buf +=  "<html><body><h1>Hello World</h1></body></html>";
                    ret = send(i, buf.c_str(), buf.size(), 0);
                    if (ret == -1)
                    {
                        std::cout << "send() failed" << std::endl;
                        FD_CLR(i, &write_set);
				        FD_CLR(i, &_fd_set);
                        close(i);
                    }
                    else
                    {
                        std::cout << "send() success"<< std::endl;
                        FD_CLR(i, &write_set);
                        FD_SET(i, &_fd_set);
                    }
                }
            }
            std::cout << "recv check" << std::endl;
            for (int i = 0; i < _max_fd + 1; ++i)
            {
                char			buff[1024];
                if (i == _fd)
                    continue;
                if (FD_ISSET(i, &fset))
                {
                    std::cout << i << std::endl;
                    int ret;
                    ret = recv(i, buff, sizeof(buff) - 1 , 0);
                    if (ret == -1)
                    {
                        std::cout << "recv() failed" << std::endl;
                        FD_CLR(i, &_fd_set);
                        close(i);
                    }
                    else if (ret == 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        FD_CLR(i, &_fd_set);
                        close(i);
                    }
                    else
                    {
                        buff[ret] = '\0';
                        std::string str(buff, ret);
	                    std::cout <<  str << std::endl;
                        FD_CLR(i, &_fd_set);
                        FD_SET(i, &write_set);
                    }
                }
            }
        }
        else
			std::cout << "didnt select" << std::endl;
    }
    return (0);
}