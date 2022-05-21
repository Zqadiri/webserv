/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:22:35 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/21 11:30:14 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEHEADERS_HPP_
#define RESPONSEHEADERS_HPP_

#include "./Response.hpp"
#include "../includes/webserv.hpp"

class ResponseHeaders
{
	private:
		std::string				_contentLength;
		std::string				_contentLanguage;
		std::string				_contentLocation;
		std::string				_contentType;
		std::string				_date;
		std::string				_lastModified;
		std::string				_location;
		std::string				_transferEncoding;
		std::string				_retryAfter;
		std::string				_server;
		std::string				_WWWAuthenticate;
		std::string				_Allow;

	public:
		ResponseHeaders();
		~ResponseHeaders();
		ResponseHeaders(const ResponseHeaders &);
		ResponseHeaders &operator=(const ResponseHeaders &);


		void					setContentLength(size_t);
		void					setContentLanguage(const std::string&);
		void					setContentLocation(const std::string&);
		void					setContentType(const std::string&);
		void					setDate(const std::string&);
		void					setLastModified(const std::string&);
		void					setLocation(const std::string&);
		void					setTransferEncoding(const std::string&);
		void					setRetryAfter(const std::string&);
		void					setServer(const std::string&);
		void					setWWWAuthenticate(const std::string&);
		void					setAllow(const std::string &str);

		std::string				writeHeader(void);
		void					initValues(void);

};

#endif
