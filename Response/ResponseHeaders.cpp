/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHeaders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 17:23:12 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/20 18:04:33 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ResponseHeaders.hpp"

ResponseHeaders::ResponseHeaders()
{
}

ResponseHeaders::~ResponseHeaders()
{
}

ResponseHeaders::ResponseHeaders(const ResponseHeaders &)
{

}

ResponseHeaders &ResponseHeaders::operator=(const ResponseHeaders &)
{
	return *this;
}

//! The length of the response body expressed in 8-bit bytes
void			ResponseHeaders::setContentLentgh(const std::string&){
	
}

//! The natural language or languages of the intended audience for the enclosed content
void			ResponseHeaders::setContentLanguage(const std::string&){
	
}

//! An alternate location for the returned data
void			ResponseHeaders::setContentLocation(const std::string&){
	
}

//! The MIME type of this content
void			ResponseHeaders::setContentType(const std::string&){
	
}

//! The date and time that the message was sent
void			ResponseHeaders::setDate(const std::string&){
	
}

//! The last modified date for the requested object
void			ResponseHeaders::setLastModified(const std::string&){
	
}

//! Used in redirection, or when a new resource has been created
void			ResponseHeaders::setLocation(const std::string&){
	
}

//! The form of encoding used to safely transfer the entity to the user.
void			ResponseHeaders::setTransferEncoding(const std::string&){
	
}

//! If an entity is temporarily unavailable, this instructs the client to try again later.
void			ResponseHeaders::setRetryAfter(const std::string&){
			
}

//! A name for the server
void			ResponseHeaders::setServer(const std::string&){
	
}

//! Indicates the authentication scheme that should be used to access the requested entity
void			ResponseHeaders::setWWWAuthenticate(const std::string&){
	
}





