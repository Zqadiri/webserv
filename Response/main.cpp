#include "Response.hpp"

int main()
{
    Response res;
    std::string str;

    str = res.ConvertHtml("./response_errors_pages/400.html");
    std::cout << str << std::endl;
}