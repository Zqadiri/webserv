/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:20:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/12 18:33:13 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include "../../includes/webserv.hpp"
#include "../server.hpp"

class request
{
	static std::vector<std::string>			possibleMethods;
	static std::vector<std::string>			init_methods();
	
	public:
		request();
		~request();

		enum ParseStatus {
			START_LINE,
			PRE_HEADERS,
			HEADERS,
			PRE_BODY,
			BODY,
			COMPLETE
		};

		enum chunkStatus{
			SIZE_LINE,
			CHUNK,
		};
		
		const std::string							&getQuery() const;
		const std::string							&getMethod() const;
		const std::string							&getPath() const;
		const std::string							&getRequestURI() const;
		const std::string							&getVersion() const;
		const std::string							&getHost() const;
		const int									&getPort() const;
		const std::map<std::string, std::string>	&getHeaders() const;
		int											getRetCode() const;

		void									setCode(int);

		void									getQuery();
		void									Host(const std::string &, request&);
		int										getFirstLine(const std::string &, request&);
		int										ParseHeaders(std::string,  request&);
		int										parseChunkedRequest(std::string);
		int										parseRquest(std::string,  request&, int);
		std::string								getNextLine(const std::string &, size_t&);
		std::string								getKey(const std::string&);
		std::string								getValue(const std::string&, size_t);
		int										checkMethod();

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
		//! print function 
		void	print_req(request &);
};


#endif
