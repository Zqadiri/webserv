#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../includes/webserv.hpp"
#include "../src/Request/request.hpp"
#include "../src/CGI/CGI.hpp"

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
        std::string     Request_statuscode_checked(request &request, serverConfig*);
        void            Methods_exec(request &request, int fd, serverConfig *);
        void            Return_string(request &request, serverConfig*, int fd);
        std::string     ConvertHtml(std::string path);
        std::string     Content_type(request &req);
        std::string     File_lenght();
        bool            isCGI();
        void            GET(int fd, request &req, serverConfig*);
        void            POST();
        void            DELETE();
        // std::fstream    get_file_stream(){return this->_file;};

    private:
        // std::fstream    _file;
        std::string     _response_string;
        int             _status_code;
        std::string     _pages_to_string;
        std::string     _file_change;
        // request     request();
};

#endif