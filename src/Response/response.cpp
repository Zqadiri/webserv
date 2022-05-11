/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwakour <nwakour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 14:00:16 by nwakour           #+#    #+#             */
/*   Updated: 2022/05/10 14:27:21 by nwakour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

response::response(const request &request, const serverConfig &server):
_code(request.getRetCode()), _type(""), _autoIndex(false), _path(""), _hostPort(), _errors()
{
	
}