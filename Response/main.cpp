#include "Response.hpp"
#include "MimeTypes.hpp"
#include <sys/stat.h>


// void File_type(std::string req)
// {

//     std::string str;
//     std::string str2;
//     int         index;

//     str = req;
//     index = str.find_first_of(".");
//     str2 = str.substr(index+1, str.length());
//     std::cout << str2 << std::endl;
//     // return ext;
// }

int File_lenght(std::string req)
{
    // we gonna calculate the length of our file (body lenght)
    int             ret;
    std::fstream    myfile(req);
    struct          stat sb;

    if(!stat(req.c_str(), &sb))
    {
        ret = sb.st_size;
        puts("1");
    }
    else
        ret = -1;
    return ret;
}

int main()
{
    int str;
    // Response res;
    str = File_lenght("./response_errors_pages/400.html");
    std::cout << str << std::endl;
}