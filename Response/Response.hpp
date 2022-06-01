#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../includes/webserv.hpp"
#include "../src/Request/request.hpp"
#include "../src/CGI/CGI.hpp"
#include "dirent.h"

class request;

class Response
{
    public:
        //canonical form
        Response();
        Response(int);
        // Response(request request);
        Response(Response &cp);
        Response & operator=(const Response &rhs);
        ~Response();

        //Response start
        std::string                 Request_statuscode_checked(request &request, serverConfig*);
        void                        Methods_exec(request &request, int fd, serverConfig *);
        void                        Return_string(request &request, serverConfig*, int fd);
        std::string                 ConvertHtml(std::string path);
        std::string                 Content_type();
        int                         File_lenght(request &req, serverConfig*, std::string);
        void                        File_type(request &req, serverConfig*);
        bool                        isCGI(request &req, serverConfig *servconf);
        void                        GET(int fd, request &req, serverConfig*);
        std::string                 getfileChange();
        std::string                 CompletePath(request &req, serverConfig*);
        std::vector<std::string>    getFilesInDirectory(std::string);
        void                        AutoIndexExec(std::string);
        std::string                 new_header_str(request &req, serverConfig *servconf);
        //!---------------- DELETE Functions --------------------------
        int                         IsFile(const std::string& path);
        int				            removeDir(std::string);
        void                        writeResponse(request &, serverConfig *,std::string);
        std::string			        getErrorPage(int	status);
        void                        DELETE(request &req, serverConfig*);

        //!---------------- POST Functions --------------------------
        void                        POST(int, request &req, serverConfig*, std::string);

    public: //!!!!! private
        std::string     _response_string;
        int             _status_code;
        std::string     _pages_to_string;
        std::string     _file_change_get;
        std::string     _file_change_delete;
        std::string     _file_extension;
        std::string     _check_extension_mine;
        bool            _get_file_success_open;
        bool            _my_auto_index;
        std::string     _s;

        std::string     _contentDisposition;
        std::string     _contentType;
        std::string     _boundary;
        std::string     header;
        std::string		str_uri;
        int             body_length;
        bool            chunked;
        std::fstream    _res;
};

#endif