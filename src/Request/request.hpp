/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:20:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/06 18:23:14 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include "../../includes/webserv.hpp"
#include "../server.hpp"

class request
{
	private:
		std::vector<std::string>				methods;
		std::string								_method;
		std::string								_requestTarget;
		std::string								_requestURI;
		std::string								_version;
		std::string								_host;
		int										_port;
		std::map<std::string, std::string>		_headers;
		std::string								_body;
		int										_retCode;

	public:
		request();
		~request();

		std::string								getMethod() const;
		std::string								getRequestURI() const;
		std::string								getVersion() const;
		std::map<std::string, std::string>		getHeaders() const;

		void									setCode(int);
		void									init_methods();
		void									Host(const std::string &, request&);
		int										getFirstLine(const std::string &, request&);
		int										startParsing(std::string,  request&);
		std::string								getNextLine(const std::string &, size_t&);
		std::string								getKey(const std::string&);
		std::string								getValue(const std::string&, size_t);

		//! print function 
		void	print_req(request &);
};

#endif
