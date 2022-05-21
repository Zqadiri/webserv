#include "Response.hpp"
#include "MimeTypes.hpp"

void File_type(std::string req)
{

    std::string str;
    std::string str2;
    int         index;

    str = req;
    index = str.find_first_of(".");
    str2 = str.substr(index+1, str.length());
    std::cout << str2 << std::endl;
    // return ext;
}
int main()
{
    // Response res;
    File_type("index.php");
    // std::cout << File_type("index.php") << std::endl;
}