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
        std::string Request_statuscode_checked(request &request, serverConfig*);
        void        Methods_exec(request &request);
        std::string Return_string(request &request);
        std::string ConvertHtml(std::string path);
        std::string Content_type();
        std::string File_lenght();
        void        GET();
        void        POST();
        void        DELETE();

    private:
        std::string _response_string;
        int         _status_code;
        std::string _pages_to_string;
        // request     request();
};

#endif