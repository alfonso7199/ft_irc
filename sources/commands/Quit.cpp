#include "Server.hpp"

void	Server::cmdQuit(int fd, const std::string &params)
{
	Client		&client = _clients.find(fd)->second;
	std::string	reason = "Quit";
	std::string	quitMsg;

	if (!params.empty())
	{
		if (params[0] == ':')
			reason = params.substr(1);
		else
			reason = params;
	}

	quitMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " QUIT :" + reason;

	std::map<std::string, Channel>::iterator
	it = _channels.begin();
	while (it != _channels.end())
	{
		if (it->second.hasMember(fd))
			it->second.broadcast(quitMsg, fd);
		++it;
	}

	disconnectClient(fd);
}
