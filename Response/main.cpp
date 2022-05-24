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

std::string time_func()
{
    time_t          rawtime;
    std::string     str;

    time(&rawtime);
    str = std::string(ctime(&rawtime));
    return str;
}

int File_lenght(std::string req)
{
    // we gonna calculate the length of our file (body lenght)
    int             ret;
    std::fstream    myfile(req);
    struct          stat sb;

    if(!stat(req.c_str(), &sb))
    {
        ret = sb.st_size;
        // puts("1");
    }
    else
        ret = -1;
    return ret;
}

bool            DELETE(std::string req)
{
    // if i understand i have to check if i have a directory or a file
    // if it's a file i have to check if i have permissions to delete it
    // if it's a directory i have to i have to check the permissions on it and then loop throught it and delete the files
    // check errnos and set the remaining status code

    struct stat s;
    if(stat(req.c_str(),&s) == 0 )
    {
        if( s.st_mode & S_IFDIR )
            return true;
        else if( s.st_mode & S_IFREG )
            return false;
    }

}

int main()
{
    std::string     str;
    int i;
    // Response res;
    // str = time_func();
    // i = File_lenght("./response_errors_pages/400.html");
    std::cout << DELETE("./response_errors_pages/") << std::endl;
}