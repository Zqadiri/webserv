#include "./Response.hpp"

int     access_files(std::string path, std::string root, std::string method)
{
    std::vector<std::string> 	ve;
	size_t 						pos = 0;
	std::string					token;
    int                         i = -1;
    std::string                 str;
    int                         flag;

    str = "";
    if(method == "GET")
        flag = R_OK;
    else if(method == "POST")
        flag = W_OK;
	while ((pos = path.find("/")) != std::string::npos) 
	{
        // puts("hhhhhhhhhhhhhhh");
		token = path.substr(0, pos);
		std::cout << token << std::endl;
		ve.push_back(token);
        path.erase(0, pos + 1);
	}
    str += root;
    std::cout << str << std::endl;
    while(++i < ve.size())
    {
        str += "/" + ve[i];
        std::cout << "str check " << str << std::endl;
        if(access(str.c_str(), F_OK) != 0)
            return 404;
        if(access(str.c_str(), flag) != 0)
            return 403;
    }
    return 200;
}

int main()
{
    std::cout << "here : "  <<  access_files("autoindex/some_images/ali.txt/", "../www", "POST")<<std::endl;
}