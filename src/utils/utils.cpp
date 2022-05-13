/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:51:02 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/13 14:53:17 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string			removeSpace(std::string init){ //! duplicate
	std::string ret;
	for (size_t i = 0; i < init.length(); ++i){
		if (!isspace(init[i]))
			ret.push_back(init[i]);
	}
	return ret;
}
