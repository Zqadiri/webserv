/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:23:12 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/20 18:21:55 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ResponseHeaders.hpp"

ResponseHeaders::ResponseHeaders(){
}

ResponseHeaders::~ResponseHeaders(){
}

ResponseHeaders::ResponseHeaders(const ResponseHeaders &obj){
	*this = obj;
}

ResponseHeaders &ResponseHeaders::operator=(const ResponseHeaders &obj)
{
	this->_contentLentgh = obj._contentLentgh;
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

//! The length of the response body expressed in 8-bit bytes
void			ResponseHeaders::setContentLentgh(size_t len){
	this->_contentLentgh = std::to_string(len);
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
