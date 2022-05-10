/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 13:30:31 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/10 16:46:55 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"
#include "../Request/request.hpp"

class response
{

	private:
	std::string							_response;
	int									_code;
	std::string							_type;
	

	const bool							_autoIndex;
	const std::string					&_path;
	const unsigned int					host;
	const int 							port;
	
	const std::map<int, std::string>	&_errors;


	// bool						_autoIndex;
	// std::string					_path;
	// // t_listen					_hostPort;
	// std::list<std::string>		_errors;
	// std::list<std::string>		_allow_methods;
	public:
	response(const request &request, const serverConfig &server);
	
};
