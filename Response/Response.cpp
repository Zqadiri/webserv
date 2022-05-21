#include "Response.hpp"

Response::Response()
{
    this->_status_code = 0;
    this->_response_string = "";
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

std::string Response::Content_type(request &req)
{
    std::string ret;
    ret = req.getContentFromMap("Content-Type");
    return ret;
}

std::string Response::File_lenght()
{
    // we gonna calculate the length of our file (body lenght)
    std::string ret;
    return ret;
}

bool    isCGI()
{
    // check if the the file is a php one and config.conf hav a cgi_pass variable
    return false;
}

void    Response::GET(int fd, request &req, serverConfig *servconf)
{
    CGI cgi_handler(req, *servconf);
    if(!isCGI())
    {
        _file_change = "/tmp/response_file_";
        _file_change += to_string(fd);
        std::fstream    myfile(_file_change);
        std::string     content_type;
        std::string     length;

        // first line in header----------------
        myfile << "HTTP/1.1 ";
        if(this->_status_code == 200)
            myfile << "200 OK\n";
        else
        {
            // should i just add the status code with an error message or should i
            // add for every error a specific status code??
            if(this->_status_code == 400)
                myfile << "400\n";
            else if(this->_status_code == 505)
                myfile << "505\n";
            else if(this->_status_code == 500)
                myfile << "500\n";
            else if(this->_status_code == 405)
                myfile << "405\n";
            else if(this->_status_code == 413)
                myfile << "413\n";
        }

        // Current Date

        // second line in header------------------
        myfile << "Content-Type: ";
        content_type = Content_type(req);
        myfile << content_type;
        myfile << "\n";

        //third line in header----------------------
        myfile << "Length: ";
        length = File_lenght();
        myfile << length;
        myfile.close();
    }
    else
        cgi_handler.executeCgi(req.getRequestURI(), fd);
}

void    Response::POST()
{
    
}

void    Response::DELETE()
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

    Request_statuscode_checked(req, servconf);
    Methods_exec(req, fd, servconf);
}

Response::~Response()
{
}