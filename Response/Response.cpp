#include "Response.hpp"

Response::Response()
{
    this._status_code = 0;
    this._response_string = "";
}

Response &Response::operator=(const Response &rhs)
{
    this._status_code = rhs._status_code;
    return (*this);
}

Response::Response(Response &cp)
{
    this->_status_code = cp._status_code;
}

void    Response::Status_code_fill()
{
    if()
    {

    }
}

Response::~Response()
{

}