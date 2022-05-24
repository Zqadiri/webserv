#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../includes/webserv.hpp"
#include "../src/Request/request.hpp"
#include "../src/CGI/CGI.hpp"

class Response
{

    public:
        //canonical form
        Response(int);
        // Response(request request);
        Response(Response &cp);
        Response & operator=(const Response &rhs);
        ~Response();

        //Response start
        std::string     Request_statuscode_checked(request &request, serverConfig*);
        void            Methods_exec(request &request, int fd, serverConfig *);
        void            Return_string(request &request, serverConfig*, int fd);
        std::string     ConvertHtml(std::string path);
        std::string     Content_type();
        int             File_lenght(request &req);
        void            File_type(request &req);
        bool            isCGI(request &req, serverConfig *servconf);
        void            GET(int fd, request &req, serverConfig*);
        void            POST();
        void            DELETE(request &req, serverConfig*);
        std::string     getfileChange();
        std::string     CompletePath(request &req, serverConfig*);
        int             IsFile(const std::string& path);
        // std::fstream    get_file_stream(){return this->_file;};

    private:
        // std::fstream    _file;
        std::string     _response_string;
        int             _status_code;
        std::string     _pages_to_string;
        std::string     _file_change_get;
        std::string     _file_change_delete;
        std::string     _file_extension;
        std::string     _check_extension_mine;
        bool            _get_file_success_open;
        // request     request();
};

#endif