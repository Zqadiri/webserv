/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:20:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/31 12:38:21 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include "../../includes/webserv.hpp"
#include "../server.hpp"
#include "../utils/utils.hpp"

class request
{
	static std::vector<std::string>			possibleMethods;
	static std::vector<std::string>			init_methods();
	
	public:
		request(int);
		~request();

		enum ParseStatus {
			START_LINE,
			PRE_HEADERS,
			HEADERS,
			PRE_BODY,
			BODY,
			CHUNKS,
			COMPLETE
		};

		enum chunkStatus{
			SIZE_LINE,
			CHUNK,
		};
		
		const std::string							&getQuery() const; // return the query string [?*]
		const std::string							&getMethod() const; //request method
		const std::string							&getPath() const; // get te full request path
		const std::string							&getRequestURI() const; // path without the query string
		const std::string							&getVersion() const; // http version
		const std::string							&getHost() const; // request Host
		std::string									&getContentFromMap(std::string); // to get content from the header map
		const int									&getPort() const; // request port
		const std::map<std::string, std::string>	&getHeaders() const; // headers 
		const std::time_t  							&getTime() const;
		int											getRetCode() const; // status code
		int											getBodyLength() const;
		int											InternalServerError();
		void										setCode(int);

		void									checkForUpload(int);
		int										parseLine(std::string line);
		void									parseAuthorization(request&);
		void									Host(const std::string &, request&);
		int										getFirstLine(const std::string &, request&);
		int										ParseHeaders(std::string,  request&);
		int										parseChunkedRequest(std::string);
		int										parseUnchunkedRequest(std::string);
		int										parseRquest(std::string,  request&, int);
		std::string								getKey(const std::string&);
		std::string								getValue(const std::string&, size_t);
		void									getQueryString();
		int										checkMethod();

		std::string								_uploadFileName;
	private:
		int										_chunkSize;
		ParseStatus 							_status;
		chunkStatus								_chunkStatus;
		std::string								_method;
		std::string								_path;
		std::string								_queryString;
		std::string								_requestURI;
		std::string								_version;
		std::string								_host;
		int										_port;
		std::map<std::string, std::string>		_headers;
		int										_retCode; 
		std::string								_tmp;
		int										_bodyLength;
		std::time_t 							_current_time;
		std::string								_contentTypeUpload;

		//! print function 
		void	print_req(request &);
};


#endif
