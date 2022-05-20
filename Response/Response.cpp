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
        // return _pages_to_string;
    // if i checked everything and the status code left is 200
    else
        _pages_to_string = "Successfull";
    return _pages_to_string;
}

void    Response::Methods_exec(request &req)
{
    std::string str;

    str = req.getMethod();
    if (this->_status_code == 200)
    {
        if(str == "GET")
            GET();
        else if(str == "POST")
            POST();
        else if(str == "DELETE")
            DELETE();
    }
}

void    Response::GET()
{

    // !!!!!! Zqadiri : stopped here to see what r u doing in that header stuff 
    std::fstream    myfile("Get_response.txt");

    myfile << "HTTP/1.1 ";
    myfile << this->_status_code;
    if(this->_status_code == 200)
        myfile << "200 OK";
    else
    {
        // should i just add the status code with an error message or should i
        // add for every error a specific status code??
    }
    myfile.close();
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

Response::~Response()
{
}