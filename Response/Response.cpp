#include "Response.hpp"
#include "MimeTypes.hpp"
#include <sys/stat.h>

Response::Response(){
}

Response::Response(int socket)
{
	std::fstream myfile;

	this->_status_code = 0;
	this->_response_string = "";
	this->_get_file_success_open = true;
	this->_file_extension = "";
	this->_s = "";
	//response file get method
	this->_file_change_get = "/tmp/response_file_get_";
	this->_file_change_get += to_string(socket);
	//response file delete method
	this->_file_change_delete = "/tmp/response_file_delete_";
	this->_file_change_delete += to_string(socket);
	this->_my_auto_index = false;
	this->header = "";
	this->body_length = 0;
	this->chunked = false;
	this->str_uri = "";
	myfile.open(_file_change_get, std::fstream::in | std::fstream::out | std::fstream::trunc);
	myfile.close();
}

Response &Response::operator=(const Response &rhs){
	this->_status_code = rhs._status_code;
	return (*this);
}

Response::Response(Response &cp){
	this->_status_code = cp._status_code;
}

std::string     			Response::Request_statuscode_checked(request &req, serverConfig* servconf)
{
	this->_status_code = req.getRetCode();
	if(this->_status_code != 200)
	{
		if(this->_status_code == 400)
			_pages_to_string = ConvertHtml("./response_errors_pages/400.html");
		else if(this-> _status_code == 505)
			_pages_to_string = ConvertHtml("./response_errors_pages/505.html");
		else if(this->_status_code == 500)
			_pages_to_string = ConvertHtml("./response_errors_pages/500.html");
		else if(req.getMethod() != "GET" || req.getMethod() != "POST" || req.getMethod() != "DELETE"){
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

void            			Response::Methods_exec(request &req, int fd, serverConfig *servconf)
{
	std::string str;

    str = req.getMethod();
    if (this->_status_code == 200)
    {
        if(str == "GET")
            return GET(fd, req, servconf);
        else if(str == "POST")
            return POST(fd, req, servconf,  "./www/upload/newFile");
        else if(str == "DELETE")
            return DELETE(req, servconf);
    }
}

std::string     			Response::Content_type()
{
    std::string ret;
    ret = std::string(this->_file_extension); //! read-memory-access (const char * to std::string)
    return ret;
}

int             			Response::File_lenght(request &req, serverConfig* servconf, std::string str)
{
	// we gonna calculate the length of our file (body lenght)
	int             ret = 0;
	struct          stat sb;
	// std::string		str;

	(void)req;
	(void)servconf;
	if(!stat(str.c_str(), &sb))
		ret = sb.st_size;
	else
		ret = -1;
	std::cout << "****** " << ret << std::endl;
	return ret;
}

std::vector<std::string>	Response::getFilesInDirectory(std::string path)
{
	DIR 						*dir;
	struct dirent				*diread;
	std::vector<std::string>	files;

	if ((dir = opendir(path.c_str())) != nullptr) 
	{
        while ((diread = readdir(dir)) != nullptr) 
            files.push_back(diread->d_name);
        closedir (dir);
    }
	else 
	{
		this->_status_code = 404;
		this->_pages_to_string = ConvertHtml("./response_errors_pages/404.html");
	}
	return files;
}

void						Response::AutoIndexExec(std::string s)
{
	int							i;
	std::fstream				file;
	std::vector<std::string>	ve;
	std::string					s2;

	i = -1;
	this->_my_auto_index = true;
	s2 = "";
	file.open("/tmp/auto_index.html",std::fstream::in | std::fstream::app);
	file << "<html>\n";
	file << "<head>\n";
	file << "<title>Index of /</title>\n";
	file << "</head>\n";
	file << "<body style=\"margin:20px\">\n";
	// file << "<center>\n";
	file << "<h1>Index of/</h1>\n";
	file << "<hr style=\"width:100%\">\n";
	// loop through the folder and get the names;
	ve = getFilesInDirectory(s);
	while(++i < (int)ve.size())
	{
		s2 = s + ve[i];
		file << "<div style=\"margin:10px\"><a href=";
		file << "\"";
		file << ve[i];
		if (IsFile(s + ve[i]) == 2)
			file << "/";
		file << "\">";
		file << ve[i];
		file << "</a></div>\n";
	}
	// file << "</center>\n";
	file << "</body>\n";
	file << "/<html>";
}

void            			Response::File_type(request &req, serverConfig *serverConfig)
{
	std::string str;
	std::string str2;
	std::string	s;
	int         index;
	const char	*type;

	s = "";
	str = req.getRequestURI();
	// std::cout << YELLOW << "path is here----------" << str << RESET << std::endl;
	index = str.find_first_of(".");
	str2 = str.substr(index + 1, str.length());
	this->_check_extension_mine = str2;
	type = MimeTypes::getType(str2.c_str());
	if(type != NULL)
		this->_file_extension = type;
}

bool            			Response::isCGI(request &req, serverConfig *servconf)
{
	(void)req;
	(void)servconf;
	// that means that the cgi pass variable is in the server config file
	if (this->_check_extension_mine == "php")
		return true;
	// check if the the file is a php one and config.conf hav a cgi_pass variable
	return false;
}

std::string     			Response::getfileChange(){
    return _file_change_get;
}

std::string					Response::CompletePath(request &req, serverConfig *servconfig)
{
	std::string				str_ret;
	std::vector<_location>	ve;
	std::string				str_req_uri;
	int						i;
	bool					check;

	str_req_uri = req.getRequestURI(); //!!!
	ve = servconfig->getLocations();
	str_ret = "";
	i = 0;
	// std::cout << YELLOW << "this is the s string here before---------- " << this->_s << RESET << std::endl;
	this->_s += servconfig->_root;
	this->_s += str_req_uri;
	// std::cout << YELLOW << "this is the s string here after----------- " << this->_s << RESET << std::endl; 
	if(IsFile(this->_s) == 2)
	{
		std::cout << RED << "im a folder here-------------" << RESET << std::endl;
		i = -1;
		check = false;
		// puts("----------------------------------2");
		while (++i < (int)ve.size())
		{ 
			if(ve[i]._path == str_req_uri)
			{
				check = true;
				str_ret += servconfig->_root;
				str_ret += ve[i]._index; // i should put here ve[i]._index
			}
		}
		if(check == false)
		{
			if(servconfig->_autoindex == false && servconfig->_index == "")
			{
				this->_status_code = 403;
				str_ret += "./Response/response_errors_pages/";
				str_ret += to_string(this->_status_code);
				str_ret += ".html";
			}
			else if(servconfig->_index != "")
			{
				str_ret += servconfig->_root;
				str_ret += servconfig->_index;
			}
			else
			{
				puts("enter auto index--------------------");
				this->_my_auto_index = true;
				AutoIndexExec(this->_s);
				str_ret = "/tmp/auto_index.html";
			}
		}
	}
	else if(IsFile(servconfig->_root + str_req_uri) == 1)
	{
		i = -1;
		check = false;
		// puts("----------------------------------1");
		while (++i < (int)ve.size())
		{
			if(ve[i]._index == str_req_uri)
			{
				check = true;
				str_ret += servconfig->_root;
				str_ret += ve[i]._index;
			}
		}
		if(check == false)
		{
			if(servconfig->_autoindex == false && servconfig->_index == "")
			{
				this->_status_code = 403;
				str_ret += "./Response/response_errors_pages/";
				str_ret += to_string(this->_status_code);
				str_ret += ".html";
			}
			else
			{
				str_ret += servconfig->_root;
				str_ret += str_req_uri;
			}
		}
	}
	else
	{
		this->_status_code = 404;
		str_ret += "./Response/response_errors_pages/";
		str_ret += to_string(this->_status_code);
		str_ret += ".html";
	}
	// std::cout << GREEN << "this is str req---------------------- " << str_req_uri << RESET << std::endl;
	return str_ret;
}

std::string     			Response::ConvertHtml(std::string path)
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

std::string					Response::getErrorPage(int	status)
{
	std::string	filename;
	filename += "./Response/response_errors_pages/";
	filename += to_string(status);
	filename += ".html";
	return ConvertHtml(filename);
}

void						Response::writeResponse(request &req, serverConfig *servconf,std::string boby)
{
	// std::fstream	myfile;
	bool			is_error = 1;

	// myfile.open(_file_change_get, std::fstream::in | std::fstream::app);
	if (this->_status_code == 204){
		is_error = 0;
		header += NO_CONTENT;
	}
	else if (this->_status_code == 403)
		header += FORBIDDEN;
	else if (this->_status_code == 201){
		is_error = 0;
		header += CREATED;
		header += "Content-Length: 0\r\n";
		body_length = 0;
	}
	else if (this->_status_code == 200){
		is_error = 0;
		header += OK;
	}
	else if (this->_status_code == 404)
		header += NOT_FOUND;
	 // end headers
	if (is_error){
		// header += getErrorPage(_status_code);
		str_uri = "./Response/response_errors_pages/";
		str_uri += to_string(_status_code);
		str_uri += ".html";
		body_length = File_lenght(req, servconf, str_uri);
		header += "Content-Length: ";
		header += to_string(body_length);
		header += "\r\n";
	}
	header += "\r\n";
	// else
	// 	header += boby;
}

int							Response::IsFile(const std::string& path)
{
	struct stat buf;
	if (stat(path.c_str(), &buf) == 0 ){
		if (buf.st_mode & S_IFDIR) // direc
			return 2;
		else if (buf.st_mode & S_IFREG) // regular file
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void            			Response::GET(int fd, request &req, serverConfig *servconf)
{
	File_type(req, servconf);
	std::string     content_type;
	time_t          rawtime;

	// if(this->_my_auto_index == false)
	str_uri = CompletePath(req, servconf);
	if(!isCGI(req, servconf))
	{
		header += "HTTP/1.1 ";
		if(this->_status_code == 200)
			header +=  "200 OK\r\n";
		else
		{
			if(this->_status_code == 400)
				header +=  "400 Bad Request\r\n";
			else if(this->_status_code == 505)
				header +=  "505 Http Version Not Supported\r\n";
			else if(this->_status_code == 500)
				header +=  "500 Internal Server Error\r\n";
			else if(this->_status_code == 405)
				header +=  "405 Not Allowed\r\n";
			else if(this->_status_code == 413)
				header +=  "413 Payload Too Large\r\n";
			else if(this->_status_code == 404)
				header +=  "404 Not Found\r\n";
			else if(this->_status_code == 403)
				header +=  "403 Forbidden\r\n";
		}

		time(&rawtime);
		header += "Date: ";
		header += std::string(ctime(&rawtime));

		header += "Server: ";
		header += "Myserver\r\n";

		header +=  "Content-Type: ";
		if(this->_status_code == 200 && this->_my_auto_index == false)
			content_type = Content_type();
		else
			content_type = "text/html";
		std::cout << RED << "content type here ---------" << Content_type() << RESET << std::endl;
		header +=  content_type;
		header +=  "\r\n"; //blanate d zineb

		body_length = File_lenght(req, servconf, str_uri);

		if(body_length + header.size() > BUFFER_SIZE)
		{
			header += "Transfer-Encoding: chuncked";
			chunked = true;
			
		}
		else
		{
			header += "Content-Length: ";
			header += to_string(body_length);
			chunked = false;
		}
		header += "\r\n\r\n";
	}
	else
	{
		CGI				cgi_handler(req, *servconf);
		cgi_handler.executeCgi(str_uri, fd, *this);
	}
}
/*
 TODO:The DELETE method
 requests that the origin server delete the resource identified by the Request-URI
*/

int							Response::removeDir(std::string path)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(path.c_str());
	if (d) 
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (!strcmp(dir->d_name, ".") ||  !strcmp(dir->d_name, ".."))
				continue;
			std::string fname = path + "/" + dir->d_name;
			int flag = IsFile(fname.c_str());
			if (flag == 1)
			{
				if (remove(fname.c_str()) == 0)
					this->_status_code = 204; // 204 (No content)
				else
					return 403;
			}
			else if (flag == 2)
				this->_status_code =  removeDir(fname);
			else
				return 403;
		}
		closedir(d);
		if (rmdir(path.c_str()) == 0)
			return (204);
		else
			return (403);
  	}
	return (403);
}

//!------------------------------------ GET ------------------------------------

// void            			Response::GET(int fd, request &req, serverConfig *servconf)
// {
// 	std::fstream    myfile;

// 	File_type(req, servconf);
// 	myfile.open(_file_change_get, std::fstream::in | std::fstream::app);
// 	this->_get_file_success_open = true;
// 	std::string     content_type;
// 	int             length(0);
// 	std::fstream    body_file; //waiting for the path
// 	std::string     fill;
// 	time_t          rawtime;

// 	// if(this->_my_auto_index == false)
// 	str_uri = CompletePath(req, servconf);
// 	if(!isCGI(req, servconf))
// 	{
// 		header = new_header_str(req, servconf);
// 	}
// 	else
// 	{
// 		CGI				cgi_handler(req, *servconf);
// 		cgi_handler.executeCgi(str_uri, fd, *this);
// 	}
// }

//!------------------------------------ DELETE ------------------------------------

void						Response::DELETE(request &req, serverConfig *servconf)
{
	(void)servconf;
	std::string f = "." + req.getPath();
	size_t isFile = IsFile(f);
	if (isFile == 1)
	{
		if (remove(f.c_str()) == 0)
			this->_status_code = 204;
		else
			this->_status_code = 403; //403 Forbidden
	}
	else if (isFile == 2){
		this->_status_code = removeDir(f);
	}
	else
		this->_status_code = 404; // 404 Not Found
	writeResponse(req,servconf,"");
}

//!------------------------------------ POST ------------------------------------

void						Response::POST(int fd, request &req, serverConfig *servconf, std::string filePath)
{
	CGI				cgi_handler(req, *servconf);
	std::string		complete_path;

	complete_path = CompletePath(req, servconf);
	cgi_handler.executeCgi(complete_path, fd, *this);
	// std::string mv = "mv " + filename + " " + "./www/upload/newfile.json";
	// system(mv.c_str());
}

void						Response::Return_string(request &req, serverConfig *servconf, int fd)
{
	Request_statuscode_checked(req, servconf);
	Methods_exec(req, fd, servconf);
}

Response::~Response()
{
	if(this->_my_auto_index == true)
		remove("/tmp/auto_index.html");
}