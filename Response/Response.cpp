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

void    Response::Status_code_fill(request &req)
{
    this->_status_code = req.getRetCode();
    if(this->_status_code != 200)
    {
        // 400 505 500 checked in request
        if(this->_status_code == 400)
        {

        }
        else if(this-> _status_code == 505)
        {

        }
        else if(this->_status_code == 500)
        {

        }
        // 403 404 405 502 left

    }
    // if i checked everything and the status code left is 200
    // else
    //     this->_status_code = 200;
}

// std::string Response::ReadHtml(std::string path)
// {
//     std::ofstream   myfile;
//     std::string     ret;

//     myfile.open(path);
//     myfile.close();
// }

Response::~Response()
{

}