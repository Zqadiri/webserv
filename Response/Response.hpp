#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "./includes/webserv.hpp"
#include "./src/Request/request.hpp"

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
        void    Status_code_fill();
        void    Start_exec();


        

    private:
        std::string _response_string;
        int         _status_code;
        request     request();
}; 

#endif