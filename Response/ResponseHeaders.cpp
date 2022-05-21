/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:23:12 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/21 12:03:49 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ResponseHeaders.hpp"

ResponseHeaders::ResponseHeaders(){
	initValues();
}

ResponseHeaders::~ResponseHeaders(){
}

ResponseHeaders::ResponseHeaders(const ResponseHeaders &obj){
	*this = obj;
}

ResponseHeaders &ResponseHeaders::operator=(const ResponseHeaders &obj){
	this->_contentLength = obj._contentLength;
	this->_contentLanguage = obj._contentLanguage;
	this->_contentLocation = obj._contentLocation;
	this->_contentType = obj._contentType;
	this->_date = obj._date;
	this->_lastModified = obj._lastModified;
	this->_location = obj._location;
	this->_transferEncoding = obj._transferEncoding;
	this->_retryAfter = obj._retryAfter;
	this->_server = obj._server;
	this->_WWWAuthenticate = obj._WWWAuthenticate;
	return *this;
}

void		ResponseHeaders::initValues(void){
	this->_contentLength = "";
	this->_contentLanguage = "";
	this->_contentLocation = "";
	this->_contentType = "";
	this->_date = "";
	this->_lastModified = "";
	this->_location = "";
	this->_transferEncoding = "";
	this->_retryAfter = "";
	this->_server = "";
	this->_WWWAuthenticate = "";
}

//! The length of the response body expressed in 8-bit bytes
void			ResponseHeaders::setContentLength(size_t len){
	this->_contentLength = to_string(len);
}

//! The natural language or languages of the intended audience for the enclosed content
void			ResponseHeaders::setContentLanguage(const std::string &str){
	this->_contentLanguage = str;
}

//! An alternate location for the returned data
void			ResponseHeaders::setContentLocation(const std::string &str){
	this->_contentLocation = str;
}

//! The MIME type of this content
void			ResponseHeaders::setContentType(const std::string &str){
	this->_contentType = str;
}

//! The date and time that the message was sent
void			ResponseHeaders::setDate(const std::string &str){
	this->_date = str;
}

//! The last modified date for the requested object
void			ResponseHeaders::setLastModified(const std::string &str){
	this->_lastModified = str;
}

//! Used in redirection, or when a new resource has been created
void			ResponseHeaders::setLocation(const std::string &str){
	this->_location = str;
}

//! The form of encoding used to safely transfer the entity to the user.
void			ResponseHeaders::setTransferEncoding(const std::string &str){
	this->_transferEncoding = str;
}

//! If an entity is temporarily unavailable, this instructs the client to try again later.
void			ResponseHeaders::setRetryAfter(const std::string &str){
	this->_retryAfter = str;
}

//! A name for the server
void			ResponseHeaders::setServer(const std::string &str){
	this->_server = str;
}

//! Indicates the authentication scheme that should be used to access the requested entity
void			ResponseHeaders::setWWWAuthenticate(const std::string &str){
	this->_WWWAuthenticate = str;
}

//! Valid methods for a specified resource. To be used for a 405 Method not allowed
void			ResponseHeaders::setAllow(const std::string &str){
	this->_Allow = str;
}

std::string		ResponseHeaders::writeHeader(void){
	std::string	header = "";

	if (_contentLanguage != "")
		header += "Content-Language: " + _contentLanguage + "\r\n";
	if (_contentLength != "")
		header += "Content-Length: " + _contentLength + "\r\n";
	if (_contentLocation != "")
		header += "Content-Location: " + _contentLocation + "\r\n";
	if (_contentType != "")
		header += "Content-Type: " + _contentType + "\r\n";
	if (_date != "")
		header += "Date: " + _date + "\r\n";
	if (_lastModified != "")
		header += "Last-Modified: " + _lastModified + "\r\n";
	if (_location != "")
		header += "Location: " + _location + "\r\n";
	if (_retryAfter != "")
		header += "Retry-After: " + _retryAfter + "\r\n";
	if (_server != "")
		header += "Server: " + _server + "\r\n";
	if (_transferEncoding != "")
		header += "Transfer-Encoding: " + _transferEncoding + "\r\n";
	if (_WWWAuthenticate != "")
		header += "WWW-Authenticate: " + _WWWAuthenticate + "\r\n";
	if (_Allow != "")
		header += "Allow: " + _Allow + "\r\n";
	return (header);
}

