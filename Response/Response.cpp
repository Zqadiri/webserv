#include "Response.hpp"
#include "MimeTypes.hpp"
#include <sys/stat.h>

std::string Response::my_root = "";

Response::Response(){
}

Response::Response(int socket)
{
	std::fstream myfile;

	this->_status_code = 200; //! i change it to 200
	this->_response_string = "";
	this->_get_file_success_open = true;
	this->_file_extension = "";
	//response file get method
	this->_file_change_get = "/tmp/response_file_get_";
	this->_file_change_get += to_string(socket);
	//response file delete method
	this->_file_change_delete = "/tmp/response_file_delete_";
	this->_file_change_delete += to_string(socket);
	this->_my_auto_index = false;
	this->header = "";
	this->body_length = 0;
	this->str_uri = "";
	this->_handled = false;
	myfile.open(_file_change_get, std::fstream::in | std::fstream::out | std::fstream::trunc);
	myfile.close();
}

Response &Response::operator=(const Response &cp){
	this->_status_code = cp._status_code;
	this->_response_string = cp._response_string;
	this->_get_file_success_open = cp._get_file_success_open;
	this->_file_extension = cp._file_extension;
	this->_pages_to_string = cp._pages_to_string;
	this->_check_extension_mine = cp._check_extension_mine;
	this->_contentDisposition = cp._contentDisposition;
	this->_contentType = cp._contentType;
	this->_file_change_get = cp._file_change_get;
	this->_file_change_delete = cp._file_change_delete;
	this->_my_auto_index = cp._my_auto_index;
	this->header = cp.header;
	this->body_length = cp.body_length;
	this->str_uri = cp.str_uri;
	this->_handled = cp._handled;
	return (*this);
}

Response::Response(const Response &cp){
	this->_status_code = cp._status_code;
	this->_response_string = cp._response_string;
	this->_get_file_success_open = cp._get_file_success_open;
	this->_file_extension = cp._file_extension;
	this->_pages_to_string = cp._pages_to_string;
	this->_check_extension_mine = cp._check_extension_mine;
	this->_contentDisposition = cp._contentDisposition;
	this->_contentType = cp._contentType;
	//response file get method
	this->_file_change_get = cp._file_change_get;
	//response file delete method
	this->_file_change_delete = cp._file_change_delete;
	this->_my_auto_index = cp._my_auto_index;
	this->header = cp.header;
	this->body_length = cp.body_length;
	this->str_uri = cp.str_uri;
	this->_handled = cp._handled;
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
            return POST(fd, req, servconf);
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

int             			Response::File_length(std::string str)
{
	// we gonna calculate the length of our file (body length)
	int             ret = 0;
	struct          stat sb;

	if(!stat(str.c_str(), &sb))
		ret = sb.st_size;
	else
		ret = -1;
	return (ret);
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
	file.open("/tmp/auto_index.html",std::fstream::out | std::fstream::trunc);
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
		// file << "/";
		file << ve[i];
		if (IsFile(s + ve[i]) == 2)
		{
			file << "/";
		}
		file << "\">";
		file << ve[i];
		file << "</a></div>\n";
	}
	// file << "</center>\n";
	file << "</body>\n";
	file << "<html>";
	file << std::endl;
	file.close();
}

void            			Response::File_type(request &req)
{
	std::string str;
	std::string str2;
	std::string	s;
	int         index;
	const char	*type;

	s = "";
	str = req.getRequestURI();
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

void						Response::File_exec(std::string *str_ret, std::string str_req_uri, std::string my_root_find)
{
	*str_ret = "";
	*str_ret += my_root_find;
	*str_ret += str_req_uri;
}

std::string					Response::CompletePath(request &req, serverConfig *servconfig)
{
	std::string				str_ret;
	std::vector<_location>	ve;
	std::string				str_req_uri;
	int						i;
	bool					check;

	str_req_uri = req.getRequestURI();
	ve = servconfig->getLocations();
	str_ret = "";
	i = 0;
	// if(IsFile(this->_s) == 2)
	// {
		i = -1;
		check = false;
		while (++i < (int)ve.size()	)
		{
			if(ve[i]._path == str_req_uri)
			{
				check = true;
				if(ve[i]._root == "")
				{
					str_ret += servconfig->_root + ve[i]._index;
					my_root = servconfig->_root;
				}
				else if((ve[i]._index == "" && ve[i]._autoindex == false))
				{
					this->_status_code = 403;
					str_ret += "./Response/response_errors_pages/";
					str_ret += to_string(this->_status_code);
					str_ret += ".html";
				}
				else
				{
					my_root = ve[i]._root;
					str_ret += ve[i]._root + ve[i]._index;
				}
				if(ve[i]._autoindex == true && ve[i]._index == "")
				{
					if(str_req_uri.find(".") != std::string::npos)
						File_exec(&str_ret, str_req_uri, ve[i]._root);
					else
					{
						AutoIndexExec(str_ret);
						str_ret = "/tmp/auto_index.html";
					}
				}
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
				str_ret += servconfig->_index;
			}
			if(servconfig->_autoindex == true && servconfig->_index == "")
			{
				if(str_req_uri.find(".") != std::string::npos)
				{
					if(my_root != "")
						File_exec(&str_ret, str_req_uri, my_root);
					else
						File_exec(&str_ret, str_req_uri, servconfig->_root);
				}
				else
				{
					puts("heree----------------------");
					std::cout << RED << "str_ret here-------------" << str_ret << RESET << std::endl;
					AutoIndexExec(str_ret + str_req_uri);
					str_ret = "/tmp/auto_index.html";
				}
			}
		}
	// }
	return str_ret;
}

std::string     			Response::ConvertHtml(std::string path)
{
	// convert Html errors pages to a string
	std::fstream	myfile;
	std::string		fill;
	std::string 	ret;

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

void						Response::writeResponse(){
	bool			is_error = 1;

	if (this->_status_code == 204){
		is_error = 0;
		header += NO_CONTENT;
	}
	if (this->_status_code == 403)
		header += FORBIDDEN;
	if (this->_status_code == 403)
		header += FORBIDDEN;
	if (this->_status_code == 201){
		is_error = 0;
		header += CREATED;
		header += "Content-Length: 0\r\n";
		body_length = 0;
	}
	if (this->_status_code == 200){
		is_error = 0;
		header += OK;
	}
	if (this->_status_code == 404)
		header += NOT_FOUND;
	if (this->_status_code == 409)
	{
		header += CONFLICT;
		is_error = 0;
		header += "Content-Length: 0\r\n";
		body_length = 0;
	}
	if (is_error){
		std::cout << "this is str_uri :: " << this->str_uri << std::endl;
		body_length = File_length(str_uri);
		header += "Content-Length: ";
		header += to_string(body_length);
		header += "\r\n";
	}
	header += "\r\n";
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

void						Response::getStatusString()
{
	if(this->_status_code == 200)
		header += OK;
	else if(this->_status_code == 400)
		header +=	BAD_REQUEST;
	else if(this->_status_code == 505)
		header +=	HTTP_version_not_supported;
	else if(this->_status_code == 500)
		header +=	INTERNAL_SERVER_ERROR;
	else if(this->_status_code == 405)
		header += 	METHOD_NOT_ALLOWED;
	else if(this->_status_code == 301)
		header +=	MOVED_PERMANENTLY;
	else if(this->_status_code == 413)
		header += 	PAYLOAD_TOO_LARGE;
	else if(this->_status_code == 404)
		header +=	NOT_FOUND;
	else if(this->_status_code == 403)
		header += 	FORBIDDEN;
}

//!------------------------------------ GET ------------------------------------

void            			Response::GET(int fd, request &req, serverConfig *servconf)
{
	File_type(req);
	std::string     content_type;
	time_t          rawtime;
	std::fstream	myfile;
	std::fstream	another_file;

	str_uri = CompletePath(req, servconf);
	myfile.open(str_uri);
	if(!myfile.is_open())
	{
		std::cout << GREEN << "str check is here--------" << servconf->_root + req.getRequestURI() << RESET << std::endl;
		another_file.open(servconf->_root + req.getRequestURI());
		if(another_file.is_open())
			str_uri = servconf->_root + req.getRequestURI();
		else
		{
			str_uri = "";
			this->_status_code = 404;
			str_uri += "./Response/response_errors_pages/";
			str_uri += to_string(this->_status_code);
			str_uri += ".html";
		}

	}
	std::cout << GREEN << "str uri is here--------" << str_uri << RESET << std::endl;
	myfile.close();
	another_file.close();
	if(!isCGI(req, servconf))
	{
		getStatusString();
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
		header +=  content_type;
		header +=  "\r\n";
		body_length = File_length(str_uri);
		header += "Content-Length: ";
		header += to_string(body_length);
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


//!------------------------------------ DELETE ------------------------------------

/*
 TODO:The DELETE method
 requests that the origin server delete the resource identified by the Request-URI
*/

void						Response::DELETE(request &req, serverConfig* servconf)
{
	(void)servconf;
	// str_uri = CompletePath(req, servconf);	
	str_uri = "." + req.getRequestURI();

	std::fstream	myfile;

	size_t isFile = IsFile(str_uri);
	if (isFile == 1 && _status_code == 200)
	{
		std::cout << "remove file" << std::endl;
		if (remove(str_uri.c_str()) == 0)
			this->_status_code = 204;
		else
			this->_status_code = 403; //403 Forbidden
	}
	else if (isFile == 2 && _status_code == 200){
		std::cout << "remove dir" << std::endl;
		if (str_uri.back() == '/')
			this->_status_code = removeDir(str_uri);
		else
			this->_status_code = 409; //409 Conflict
	}
	else
		this->_status_code = 404; // 404 Not Found
	writeResponse();
}

//!------------------------------------ POST ------------------------------------

/*
	TODO:The POST method
	requests that the origin server accept the entity enclosed in the request as a new subordinate
	of the resource identified by the Request-URI in the Request-Line
*/

bool		Response::supportUpload(request &req,  serverConfig *servconf)
{
	std::vector<_location> ve = servconf->getLocations();
	std::string uri = req.getRequestURI();
	std::string loc = uri.substr(0, uri.find("newfile"));
	std::cout << "loc is " << loc << std::endl;
	for (std::vector<_location>::iterator it = ve.begin(); it != ve.end(); ++it){
		std::string path = it->_path += "/";
		if (path == loc){
			_uploadFileName = it->_uploadStore;
			return true;
		}
	}
	return false;
}

int			Response::parseLine(std::string line)
{
	std::string key, value;

	if (line.compare("\r\n"))
	{
		if (line.find("Content-Disposition") != std::string::npos ||
				line.find("Content-Type") != std::string::npos)
		{
			if (line.find("Content-Disposition") != std::string::npos)
			{
				size_t end = line.find("filename=");
				_uploadFileName = line.substr(end + 9, line.size());
				std::cout << "upload file name: " << _uploadFileName << std::endl;
			}
			return 0;
		}
	}
	if (line.find("------") != std::string::npos)
		return 0;
	return 1;
}

void						Response::POST(int fd, request &req, serverConfig *servconf)
{
	(void)fd;
	std::string		complete_path;
	std::string		location;
	std::string		filename = randomFileName();
	std::fstream	newBody;

	complete_path = CompletePath(req, servconf);
	newBody.open(filename.c_str(), std::fstream::in | std::fstream::app);
	std::cout << "POST: " << _status_code << std::endl;
	File_type(req);
	if (supportUpload(req, servconf) && _status_code == 200)
	{
		std::cout << GREEN << "------------------- UPLOAD -----------------------" << RESET << std::endl;
		std::fstream reqBody;
		std::string	line;
		reqBody.open("./tmp/body"+ to_string(fd), std::fstream::in);
		while (reqBody){
			std::getline(reqBody, line);
			if (parseLine(line)){
				newBody << line;
				newBody << "\n";
			}
		}
		newBody.close();
		reqBody.close();
		std::string mv = "mv " + filename  + " ./www/upload/newfile";
		system(mv.c_str());
		return ;
	}
	else if (isCGI(req, servconf) && _status_code == 200)
	{
		std::cout << GREEN << "------------------- CGI -----------------------" << RESET << std::endl;
		CGI				cgi_handler(req, *servconf);
		cgi_handler.executeCgi(complete_path, fd, *this);
	}
	else
	{
		std::cout << GREEN << "------------------- ERROR -----------------------" << RESET << std::endl;
		this->_status_code = 404;
		str_uri += "./Response/response_errors_pages/";
		str_uri += to_string(this->_status_code);
		str_uri += ".html";
	}

}
void						Response::Return_string(request &req, serverConfig *servconf, int fd)
{
	Request_statuscode_checked(req, servconf);
	Methods_exec(req, fd, servconf);
	_handled = true;
}

Response::~Response()
{
	if(this->_my_auto_index == true)
		remove("/tmp/auto_index.html");
}