/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zqadiri <zqadiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:50:52 by zqadiri           #+#    #+#             */
/*   Updated: 2022/05/30 17:43:47 by zqadiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "../../includes/webserv.hpp"

std::string			removeSpace(std::string init);
std::string         to_string(size_t n);
std::string			getNextLine(const std::string &buff, size_t &cursor);

#endif 
