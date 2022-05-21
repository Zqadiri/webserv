
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



int main()
{
    std::time_t start = std::time(NULL);
    sleep(10);
    std::time_t end = std::time(NULL);
    

    std::cout << "time passed: " << end - start << std::endl;

    return (0);
}