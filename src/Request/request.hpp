/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:20:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/08 18:18:44 by zqadiri          ###   ########.fr       */
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
		std::string								_body;

	public:
		request();
		~request();

		std::string								getQuery() const;
		std::string								getMethod() const;
		std::string								getPath() const;
		std::string								getRequestURI() const;
		std::string								getVersion() const;
		std::string								getHost() const;
		int										getPort() const;
		std::map<std::string, std::string>		getHeaders() const;
		int										getRetCode() const;

		void									setBody(const std::string& str);
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
