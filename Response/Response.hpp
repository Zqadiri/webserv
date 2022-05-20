#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../includes/webserv.hpp"
#include "../src/Request/request.hpp"

class Response
{
    public:
        //canonical form
        Response();
        // Response(request request);
        Response(Response &cp);
        Response & operator=(const Response &rhs);
        ~Response();

        //Response start
        std::string Request_status_checked(request &request, serverConfig*);
        void        Start_exec();
        std::string Return_string(request &request, serverConfig*);
        std::string ConvertHtml(std::string path);

    private:
        std::string _response_string;
        int         _status_code;
        std::string _pages_to_string;
        // request     request();
}; 

#endif