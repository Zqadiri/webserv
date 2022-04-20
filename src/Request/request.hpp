/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 00:20:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/04/20 00:01:31 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include "../../includes/webserv.hpp"
#include "../server.hpp"

class request
{
	private:
		std::string								_method; //! GET POST DELETE
		std::string								_requestURI;
		std::string								_version; //! http 1.0 1.1
		std::map<std::string, std::string>		_headers;
		std::vector<std::string>				methods; //! no need to be global
		std::string								_body;

		int										_retCode; 
	public:
		request();
		~request();

		std::string								getMethod();
		std::string								getRequestURI();
		std::string								getVersion();
		std::map<std::string, std::string>		getHeaders();
		
		void									init_methods();
		int										getFirstLine(const std::string &);
		std::string								getNextLine(const std::string &, int&);
		int										startParsing(std::string);
};

#endif
