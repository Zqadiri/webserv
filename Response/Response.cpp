#include "Response.hpp"
#include "MimeTypes.hpp"
#include <sys/stat.h>

Response::Response()
{
    this->_status_code = 0;
    this->_response_string = "";
    this->_get_file_success_open = true;
}

Response &Response::operator=(const Response &rhs){
    this->_status_code = rhs._status_code;
    return (*this);
}

Response::Response(Response &cp)
{
    this->_status_code = cp._status_code;
}

std::string    Response::Request_statuscode_checked(request &req, serverConfig* servconf)
{
    this->_status_code = req.getRetCode();
    if(this->_status_code != 200)
    {
        // 400 505 500 checked in request and 405 413 checked in here
        if(this->_status_code == 400)
            _pages_to_string = ConvertHtml("./response_errors_pages/400.html");
        else if(this-> _status_code == 505)
            _pages_to_string = ConvertHtml("./response_errors_pages/505.html");
        else if(this->_status_code == 500)
            _pages_to_string = ConvertHtml("./response_errors_pages/500.html");
        else if(req.getMethod() != "GET" || req.getMethod() != "POST" || req.getMethod() != "DELETE")
        {
            this->_status_code = 405;
            _pages_to_string = ConvertHtml("./response_errors_pages/405.html");
        }
        else if(servconf->getlimitBodySize() != -1 && req.getBodyLength() > servconf->getlimitBodySize())
        {
            this->_status_code = 413;
            _pages_to_string = ConvertHtml("./response_errors_pages/413.html");
        }
    }
    else
        _pages_to_string = "Successfull";
    return _pages_to_string;
}

void    Response::Methods_exec(request &req, int fd, serverConfig *servconf)
{
    std::string str;

    str = req.getMethod();
    if (this->_status_code == 200)
    {
        if(str == "GET")
            return GET(fd, req, servconf);
        else if(str == "POST")
            return POST();
        else if(str == "DELETE")
            return DELETE();
    }
}

std::string Response::Content_type()
{
    std::string ret;
    ret = this->_file_extension;
    return ret;
}

int         Response::File_lenght(request &req)
{
    // we gonna calculate the length of our file (body lenght)
    int             ret;
    struct          stat sb;

    if(!stat(req.getRequestURI().c_str(), &sb))
        ret = sb.st_size;
    else
        ret = -1;
    return ret;
}

void    Response::File_type(request &req)
{

    std::string str;
    std::string str2;
    int         index;

    str = req.getRequestURI();
    index = str.find_first_of(".");
    str2 = str.substr(index+1, str.length());
    this->_check_extension_mine = str2;
    this->_file_extension = MimeTypes::getType(str.c_str());
}

bool    Response::isCGI(request &req, serverConfig *servconf)
{
    (void)req;
    (void)servconf;
    // that means that the cgi pass variable is in the server config file
    if (this->_check_extension_mine == "php")
        return true;
    // check if the the file is a php one and config.conf hav a cgi_pass variable
    return false;
}

void    Response::GET(int fd, request &req, serverConfig *servconf)
{
    // CGI cgi_handler(req, *servconf);
    if(!isCGI(req, servconf))
    {
        _file_change = "/tmp/response_file_";
        _file_change += to_string(fd);
        std::fstream    myfile(_file_change);
        std::string     content_type;
        int             length;
        std::fstream    body_file; //waiting for the path
        std::string     fill;
        time_t          rawtime;

        this->_get_file_success_open = true;
        // first line in header----------------
        myfile << "HTTP/1.1 ";
        if(this->_status_code == 200)
            myfile << "200 OK\r\n";
        else
        {
            if(this->_status_code == 400)
                myfile << "400 Bad Request\r\n";
            else if(this->_status_code == 505)
                myfile << "505 Http Version Not Supported\r\n";
            else if(this->_status_code == 500)
                myfile << "500 Internal Server Error\r\n";
            else if(this->_status_code == 405)
                myfile << "405 Not Allowed\r\n";
            else if(this->_status_code == 413)
                myfile << "413 Payload Too Large\r\n";
        }

        // Current Date-----------------
        time(&rawtime);
        myfile << "Date: ";
        myfile << std::string(ctime(&rawtime));

        //Server------------------------
        myfile << "Server: ";
        myfile << "Myserver\r\n";

        //Content Type------------------
        myfile << "Content-Type: ";
        content_type = Content_type();
        myfile << content_type;
        myfile << "\r\n"; //blanate d zineb

        //Body length----------------------
        myfile << "Content-Length: ";
        length = File_lenght(req);
        myfile << to_string(length);

        //blank line
        myfile << "\r\n";

        //Body part start
        body_file.open("path");
        if(body_file)
        {
            while(!body_file.eof())
            {
                std::getline(body_file, fill);
                myfile << fill;
                myfile << "\n";
            }
        }
        else
            this->_get_file_success_open = false;

        //end of method and close file    
        myfile.close();
    }
    // else
    //     cgi_handler.executeCgi(req.getRequestURI(), fd);
}

void        Response::POST()
{
    
}

void        Response::DELETE()
{
    
}

std::string Response::ConvertHtml(std::string path)
{
    // convert Html errors pages to a string
    std::fstream    myfile;
    std::string     fill;
    std::string     ret;

    myfile.open(path);
    while(myfile)
    {
        std::getline(myfile, fill);
        ret += fill;
        ret += "\n";
    }
    myfile.close();
    return (ret);
}

void    Response::Return_string(request &req, serverConfig *servconf, int fd)
{
	// std::cout << servconf->_root << std::endl;
    Request_statuscode_checked(req, servconf);
    Methods_exec(req, fd, servconf);
}

Response::~Response()
{
}