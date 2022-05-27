
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

#include <ctime>
#include <sys/time.h>



int main(int argc, char *argv[], char* env[])
{
    pid_t fd;

    fd = fork();
    if (fd == 0){
      execle("./php-cgi", "./www/big_hello.php", NULL, env);
	  	perror(" ->");
    }
    else{
	  	waitpid(-1, NULL, 0);
    }
    return (0);
}