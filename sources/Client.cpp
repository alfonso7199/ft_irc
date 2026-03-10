/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 18:22:39 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/10 18:28:38 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(std::string nickname, std::string realname) :
_nickname(nickname), _realname(realname)
{
	return ;
}

Client::Client(const Client &other) : _nickname(other._nickname), 
_realname(other._realname)
{
	return ;
}

Client	&Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->_realname = other._realname;
		this->_nickname = other._nickname;
	}
	return (*this);
}

Client::~Client(void)
{
	return ;
}

const std::string	&Client::getRealname(void) const
{
	return (this->_realname);
}

const std::string	&Client::getNickname(void) const
{
	return (this->_nickname);
}

std::ostream	&operator<<(std::ostream &os, const Client &c)
{
	os << c.getNickname() << ": " << c.getRealname();
	return (os);
}
