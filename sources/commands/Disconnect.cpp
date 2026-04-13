#include "Server.hpp"

void	Server::disconnectClient(int fd)
{
	std::map<int, Client>::iterator	clientIt = _clients.find(fd);
	if (clientIt == _clients.end())
		return ;

	Client		&client = clientIt->second;
	std::string	quitMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " QUIT :connection closed";

	std::map<std::string, Channel>::iterator
	chanIt = _channels.begin();
	while (chanIt != _channels.end())
	{
		if (chanIt->second.hasMember(fd))
		{
			chanIt->second.broadcast(quitMsg, fd);
			chanIt->second.removeMember(fd);
			if (chanIt->second.getMemberCount() == 0)
			{
				_channels.erase(chanIt++);
				continue ;
			}
		}
		++chanIt;
	}

	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == fd)
		{
			_pollfds.erase(_pollfds.begin() + i);
			break ;
		}
	}

	close(fd);
	_clients.erase(fd);
}
