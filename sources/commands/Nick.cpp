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
#include <set>

static bool	isValidNick(const std::string &nick)
{
	if (nick.empty() || nick.size() > 30)
		return (false);

	unsigned char	first = nick[0];
	if (first == '#' || first == '&' || first == ':' || first == '@' || std::isdigit(first))
		return (false);

	for (size_t i = 0; i < nick.size(); i++)
	{
		unsigned char	c = nick[i];
		if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!' || c == '@')
			return (false);
	}
	return (true);
}

void	Server::cmdNick(int fd, const std::string &params)
{
	Client	&client = _clients.find(fd)->second;

	if (params.empty())
	{
		sendReply(fd, ":" + _name + ERR_NONICKNAMEGIVEN + "* :No nickname given");
		return ;
	}

	std::string	newNick = params;
	if (!newNick.empty() && newNick[0] == ':')
		newNick = newNick.substr(1);

	if (!isValidNick(newNick))
	{
		sendReply(fd, ":" + _name + ERR_ERRONEUSNICKNAME + "* " + newNick + " :Erroneous nickname");
		return ;
	}

	// Comprobar que el nick no está ya en uso
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first != fd && it->second.getNickname() == newNick)
		{
			sendReply(fd, ":" + _name + ERR_NICKNAMEINUSE + "* " + newNick + " :Nickname is already in use");
			return ;
		}
	}

	const std::string	oldNick = client.getNickname();
	client.setNickname(newNick);

	if (!client.isRegistered())
	{
		tryRegister(fd);
		return ;
	}
	const std::string	prefix = ":" + oldNick + "!" + client.getUsername() + "@" + client.getHostname();
	const std::string	nickMsg = prefix + " NICK " + newNick;

	std::set<int>	notified;
	sendReply(fd, nickMsg);
	notified.insert(fd);

	for (std::map<std::string, Channel>::iterator chIt = _channels.begin(); chIt != _channels.end(); ++chIt)
	{
		if (!chIt->second.hasMember(fd))
			continue ;

		std::map<int, Client *>	&members = chIt->second.getMembers();
		for (std::map<int, Client *>::iterator mIt = members.begin(); mIt != members.end(); ++mIt)
		{
			if (notified.count(mIt->first) == 0)
			{
				sendReply(mIt->first, nickMsg);
				notified.insert(mIt->first);
			}
		}
	}
}
