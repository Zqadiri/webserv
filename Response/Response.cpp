#include "Response.hpp"

Response::Response()
{
    this->_status_code = 0;
    this->_response_string = "";
}

Response &Response::operator=(const Response &rhs)
{
    this->_status_code = rhs._status_code;
    return (*this);
}

Response::Response(Response &cp)
{
    this->_status_code = cp._status_code;
}

std::string    Response::Request_status_checked(request &req, serverConfig* servconf)
{
    this->_status_code = req.getRetCode();
    if(this->_status_code != 200)
    {
        // 400 505 500 checked in request
        if(this->_status_code == 400)
            _pages_to_string = ConvertHtml("./response_errors_pages/400.html");
        else if(this-> _status_code == 505)
            _pages_to_string = ConvertHtml("./response_errors_pages/505.html");
        else if(this->_status_code == 500)
            _pages_to_string = ConvertHtml("./response_errors_pages/500.html");
        else if(servconf.get)
        return _pages_to_string;
    }
    // if i checked everything and the status code left is 200
    // else
    //     this->_status_code = 200;
}

std::string Response::ConvertHtml(std::string path)
{
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

Response::~Response()
{
}