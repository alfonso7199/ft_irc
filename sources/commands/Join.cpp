#include "Server.hpp"

static std::string	buildNamesList(Channel &channel)
{
	std::string						names;
	std::map<int, Client *>			&members = channel.getMembers();
	std::map<int, Client *>::iterator	it = members.begin();

	while (it != members.end())
	{
		if (channel.isAdmin(it->first))
			names += "@";
		names += it->second->getNickname();
		++it;
		if (it != members.end())
			names += " ";
	}
	return (names);
}

void	Server::cmdJoin(int fd, const std::string &params)
{
	Client		&client = _clients.find(fd)->second;
	std::string	channelName;
	std::string	key;
	size_t		space = params.find(' ');

	if (!client.isRegistered())
	{
		sendReply(fd, ":" + _name + ERR_NOTREGISTERED + "* :You have not registered");
		return ;
	}

	if (space != std::string::npos)
	{
		channelName = params.substr(0, space);
		key = params.substr(space + 1);
	}
	else
		channelName = params;

	if (channelName.empty() || channelName[0] != '#')
	{
		sendReply(fd, ":" + _name + ERR_NOSUCHCHANNEL + client.getNickname() + " " + channelName + " :No such channel");
		return ;
	}

	if (_channels.count(channelName))
	{
		Channel	&channel = _channels.find(channelName)->second;

		if (channel.hasMember(fd))
			return ;

		if (channel.isInviteOnly() && !channel.isInvited(fd))
		{
			sendReply(fd, ":" + _name + ERR_INVITEONLYCHAN + client.getNickname() + " " + channelName + " :Cannot join channel (+i)");
			return ;
		}

		if (!channel.getPasswordChannel().empty() && channel.getPasswordChannel() != key)
		{
			sendReply(fd, ":" + _name + ERR_BADCHANNELKEY + client.getNickname() + " " + channelName + " :Cannot join channel (+k)");
			return ;
		}

		if (channel.getUserLimit() > 0 && channel.getMemberCount() >= channel.getUserLimit())
		{
			sendReply(fd, ":" + _name + ERR_CHANNELISFULL + client.getNickname() + " " + channelName + " :Cannot join channel (+l)");
			return ;
		}

		channel.addMember(&client);
	}
	else
	{
		_channels.insert(std::make_pair(channelName, Channel(channelName)));
		Channel	&channel = _channels.find(channelName)->second;
		channel.addMember(&client);
		channel.addAdmin(fd);
	}

	Channel		&channel = _channels.find(channelName)->second;
	std::string	prefix = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();

	channel.broadcast(prefix + " JOIN " + channelName);

	if (channel.getTopic().empty())
		sendReply(fd, ":" + _name + RPL_NOTOPIC + client.getNickname() + " " + channelName + " :No topic is set");
	else
		sendReply(fd, ":" + _name + RPL_TOPIC + client.getNickname() + " " + channelName + " :" + channel.getTopic());

	sendReply(fd, ":" + _name + RPL_NAMREPLY + client.getNickname() + " = " + channelName + " :" + buildNamesList(channel));
	sendReply(fd, ":" + _name + RPL_ENDOFNAMES + client.getNickname() + " " + channelName + " :End of /NAMES list");
}