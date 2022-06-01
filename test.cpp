

#include <cstring>
#include <iostream>
int main()
{
    int  num = 32424;
    char hex[2000];
    hex[0] = 'a';
    hex[1] = 'b';
    hex[2] = 'c';
    hex[3] = 'd';
    hex[4] = ' ';

    sprintf(hex + 5 , "%x", num);

    std::cout << hex << std::endl;
}