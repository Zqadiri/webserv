/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:20:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/10 14:29:09 by zqadiri          ###   ########.fr       */
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
	
	private:
		std::string								_method;
		std::string								_path;
		std::string								_queryString;
		std::string								_requestURI;
		std::string								_version;
		std::string								_host;
		int										_port;
		std::map<std::string, std::string>		_headers;
		int										_retCode;
		FILE*									_body;
		std::string								_tmp;
		bool 									_header_finished;
	
	public:
		request();
		~request();

		const std::string							&getQuery() const;
		const std::string							&getMethod() const;
		const std::string							&getPath() const;
		const std::string							&getRequestURI() const;
		const std::string							&getVersion() const;
		const std::string							&getHost() const;
		const int									&getPort() const;
		const std::map<std::string, std::string>	&getHeaders() const;
		const int									&getRetCode() const;

		void									setCode(int);

		void									getQuery();
		void									Host(const std::string &, request&);
		int										getFirstLine(const std::string &, request&);
		int										startParsing(std::string,  request&);
		std::string								getNextLine(const std::string &, size_t&);
		std::string								getKey(const std::string&);
		std::string								getValue(const std::string&, size_t);
		int 									checkData(std::string buff,  request& r, size_t);
		int										checkMethod();


		//! print function 
		void	print_req(request &);
};


#endif
