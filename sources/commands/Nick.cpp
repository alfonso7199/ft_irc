/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 19:18:20 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/04/15 19:18:31 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::cmdNick(int fd, const std::string &params)
{
	Client							&client = this->_clients.find(fd)->second;
	std::map<int, Client>::iterator	it = this->_clients.begin();

	if (params.empty())
	{
		sendReply(fd, ":" + this->_name + ERR_NONICKNAMEGIVEN + "* :No nickname given");
		return ;
	}
	while (it != this->_clients.end())
	{
		if (it->first != fd && it->second.getNickname() == params)
		{
			sendReply(fd, ":" + this->_name + ERR_NICKNAMEINUSE + "* " + params + " :Nickname is already in use");
			return ;
		}
		++it;
	}
	client.setNickname(params);
	tryRegister(fd);
}
